#include <crime_data.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <question.h>

namespace chicago_crime{

CrimeData::CrimeData(string _crime_type, LocationType _exterior, int _hour, int _district) :
    crime_type(_crime_type), exterior(_exterior), hour(_hour), district(_district)
{}

CrimeData CrimeDataPack::operator[](unsigned long i){
    return data_.at(i);
}

void CrimeDataPack::append(CrimeData cd){
    data_.push_back(cd);
}


double_cdp CrimeDataPack::partition(Question q){
    /* function that compares every element in
     * the container with given question and returns
     * false and true parts.
     */
    CrimeDataPack true_cdp, false_cdp;
    for(auto& cd : data_){
        if(q.ask(cd))
            true_cdp.append(cd);
        else
            false_cdp.append(cd);
    }
    return std::make_tuple(true_cdp,false_cdp);
}

unordered_map<string,int> CrimeDataPack::count_labels(){
    /* function that counts every label in the container
     */
    unordered_map<string,int> labels;
    for(auto& cd : data_){
        auto search = labels.find(cd.crime_type);
        if(search == labels.end())
            labels.insert({cd.crime_type,1});
        else
            (search->second)++;
    }
    return labels;
}

double CrimeDataPack::gini_impurity(){
    /* gini impurity funtion. more information:
     * {https://en.wikipedia.org/wiki/Decision_tree_learning}
     */
    auto labels = count_labels();
    double gini = 1.;
    for(auto p : labels)
        gini -= std::pow(double(p.second) / data_.size(), 2.);
    return gini;
}


triple_set CrimeDataPack::unique_vals(){
    /* function that extracts every
     * unique feature value in the data set
     * for every feature
     */
    unordered_set<int> time, dist;
    unordered_set<LocationType> loc;
    for(auto& cd : data_){
        time.insert(cd.hour);
        dist.insert(cd.district);
        loc.insert(cd.exterior);
    }
    return std::make_tuple(time,dist,loc);
}

string CrimeDataPack::results(){
    /* utility function */
    auto stats = count_labels();
    vector<std::pair<string,int> > values(stats.begin(), stats.end());
    std::sort(values.begin(), values.end(),
              [](std::pair<string,int> &left, std::pair<string,int> &right) {
        return left.second > right.second;
    });
    vector<string> out;
    for(auto& p : values){
        std::stringstream ss;
        ss << p.first
           << " : <font color=\"red\">"
           << std::fixed
           << std::setprecision(2)
           << double(p.second)/size()*100
           << " %</font><br>";
        out.push_back(ss.str());
    }

    return std::accumulate(out.begin(),out.end(),string("<font size=6 color=\"blue\">RESULTS:</font><br><br>"));
}

}
