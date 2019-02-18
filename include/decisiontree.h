#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <tuple>
#include <memory>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include <question.h>
#include <crime_data.h>

namespace chicago_crime {

using std::string;
using std::tuple;
using std::unique_ptr;
using std::unordered_set;

class DecisionTree {
public:
    DecisionTree(){}
    DecisionTree(CrimeDataPack);

    /* function to classify given crimedata */
    CrimeDataPack classify(CrimeData&);
    /*  utility printing function*/
    void print_tree(string);
private:
    /* function that finds the optimum question and its gain */
    tuple<double,Question> splitData(CrimeDataPack&);

    CrimeDataPack data_;
    Question question_;
    bool terminal_;

    /* pointers for child trees */
    unique_ptr<DecisionTree> t_, f_;

    /* utility function */
    friend class boost::serialization::access;
    template<class A>
    void serialize(A& ar, const unsigned int version){
        ar & data_ & terminal_ & t_ & f_ & question_;
    }};

}


#endif // DECISIONTREE_H

