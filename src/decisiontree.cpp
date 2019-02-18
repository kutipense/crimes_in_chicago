#include <decisiontree.h>
#include <iostream>

namespace chicago_crime {

double information_gain(CrimeDataPack& l, CrimeDataPack& r, double impurity){
    /* information gain calculations with gini impurity
     * {https://en.wikipedia.org/wiki/Decision_tree_learning}
     */
    double p = double(l.size()) / (l.size() + r.size());
    return impurity - p*l.gini_impurity() - (1-p)*r.gini_impurity();
}

DecisionTree::DecisionTree(CrimeDataPack crime_data_pack) : terminal_(0) {
    /* recursive constructor to build decision tree
     */
    double gain;
    Question question;
    // find optimal question to seperate data
    std::tie(gain, question) = splitData(crime_data_pack);
    question_ = question;

    /* if gain == 0 stop and save the sub_data
     * (which means there is no other question to ask
     * in order to seperate data)
     */
    if(std::abs(gain) < 0.0000001){
        terminal_ = 1;
        data_ = crime_data_pack;
        return;
    }

    CrimeDataPack t,f;
    // seperate data with the optimal question
    std::tie(t,f) = crime_data_pack.partition(question);
    unique_ptr<DecisionTree> t_p(new DecisionTree(t));
    unique_ptr<DecisionTree> f_p(new DecisionTree(f));
    // build tree for those sub datas
    t_ = std::move(t_p);
    f_ = std::move(f_p);

}

std::tuple<double,Question> DecisionTree::splitData(CrimeDataPack& crime_data_pack){
    /* function that test every feature's questions in order
     * to seperate the data in the optimum way
     */
    Question question;
    double max_gain = 0;
    // get impurity of base data
    double curr_impurity = crime_data_pack.gini_impurity();

    // acquire unique features and create questions from them
    auto unique_vals = crime_data_pack.unique_vals(); //time,district
    unordered_set<int> time_vals;
    unordered_set<int> district_vals;
    unordered_set<LocationType> location_vals;

    std::tie(time_vals, district_vals, location_vals) = unique_vals;

    std::vector<Question> question_list;
    for(auto& v: time_vals) question_list.push_back(Question(v, TIME));
    for(auto& v: district_vals) question_list.push_back(Question(v, DISTRICT));
    for(auto& v: location_vals) question_list.push_back(Question(v));
    // end of question creation

    /* try every question and find the optimum question that
    *  seperates the data most homogeneously.
    */
    for(auto& q : question_list){
        CrimeDataPack l, r;
        std::tie(l,r) = crime_data_pack.partition(q);

        // question not seperating data (not effective)
        if(l.is_empty() || r.is_empty()) continue;

        double gain = information_gain(l,r,curr_impurity);

        if(gain>=max_gain){
            question = q;
            max_gain = gain;
        }
    }
    return std::make_tuple(max_gain,question);
}

CrimeDataPack DecisionTree::classify(CrimeData& cd){
    // recursive funtion to classify given crime data;
    if(terminal_)
        return data_;
    return question_.ask(cd) ? t_->classify(cd) : f_->classify(cd);

}

void DecisionTree::print_tree(string space){
    /* printing utility function */
    if(terminal_){
        std::cout << space << "sonuclar " << std::endl;
        return;
    }
    std::cout << space << question_.get_desc() << std::endl;

    std::cout << space << "true branch:" << std::endl;
    t_->print_tree(space+"|  ");

    std::cout << space << "false branch:" << std::endl;
    f_->print_tree(space+"|  ");
}

}
