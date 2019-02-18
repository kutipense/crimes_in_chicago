#include "mywindow.h"
#include <QApplication>
#include <csv_parser.h>
#include <crime_data.h>
#include <iostream>
#include <decisiontree.h>
#include <question.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace aria::csv;
using std::string;

using chicago_crime::CrimeData;
using chicago_crime::CrimeDataPack;
using chicago_crime::DecisionTree;

CrimeDataPack parse_csv(string filename){
    /* csv parser */
    std::ifstream f(filename);
    CsvParser parser = CsvParser(f).delimiter(';').quote('\0');
    CrimeDataPack crime_pack;

    for (auto row : parser) {
        crime_pack.append(
            CrimeData(
                row[0],
                std::stoi(row[2]) == 0
                    ? chicago_crime::EXTERIOR
                    : chicago_crime::INTERIOR ,
                std::stoi(row[1]),
                std::stoi(row[3])
            )
        );
    }
    return crime_pack;
}

DecisionTree calc_tree(){
    /* function that builds a decision tree and stores to a file */
    CrimeDataPack cdp = parse_csv("/home/kuti/Desktop/uzman/crime_data_train.csv");
    DecisionTree dt(cdp);
    dt.print_tree("");

    std::ofstream ofs("dt.tree");
    {
        boost::archive::text_oarchive oa(ofs);
        oa << dt;
    }
    return dt;
}

DecisionTree read_tree(){
    /* function to read the decision tree from file */
    DecisionTree dt;
    {
        std::ifstream ifs("decision_tree.txt");
        boost::archive::text_iarchive ia(ifs);
        ia >> dt;
    }
    //dt.print_tree("");
    return dt;
}

namespace chicago_crime {
tuple<double,double> test_tree(DecisionTree& dt){
    /* success rate test function
     * calculates the percentage of true predictions
     * and averae suggestion rate of those predictions
     */
    CrimeDataPack cdp = parse_csv("/home/kuti/Desktop/uzman/crime_data_test.csv");

    double true_vals = 0;
    double rate = 0;
    int count = 0;
    for(auto& cd : cdp.data_){
        auto classified = dt.classify(cd);
        auto probs = classified.count_labels();
        auto it = probs.begin();
        bool found = false;
        while(!found && it!=probs.end()){
            /* if case suggested calculate ratios */
            if(it->first==cd.crime_type){
                found = true;
                true_vals++;
                rate += double(it->second) / classified.size() * 100;
                count++;
            }
            it++;
        }
    }
    return std::make_tuple(true_vals / cdp.size() * 100, rate / count);
}
}

int main(int argc, char *argv[])
{
    //auto dt = calc_tree();

    auto dt = read_tree();

    /*double true_vals, rate;
    std::tie(true_vals,rate) = chicago_crime::test_tree(dt);
    std::cout << true_vals << "% success rate with average of " << rate << "%" << std::endl;*/

    QApplication a(argc, argv);
    MyWindow w;
    w.set_dt(&dt);
    w.show();
    return a.exec();
}
