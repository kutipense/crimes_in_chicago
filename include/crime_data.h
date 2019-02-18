#ifndef CRIME_DATA_H
#define CRIME_DATA_H

#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <typedefs.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#define ROWS 38
#define COLS 23

namespace chicago_crime{

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::tuple;

class Question;
class CrimeDataPack;
class DecisionTree;

typedef tuple<unordered_set<int>,unordered_set<int>,unordered_set<LocationType> > triple_set;
typedef tuple<CrimeDataPack,CrimeDataPack > double_cdp;

struct CrimeData{
    /*  crime data structre */
    string crime_type;
    LocationType exterior;
    int hour;
    int district;

    CrimeData(){}
    CrimeData(string, LocationType, int, int);

    /* utility definitions for serialization of decision tree */
    friend class boost::serialization::access;
    template<class A>
    void serialize(A& ar, const unsigned int version){
        ar & crime_type & exterior & hour & district;
    }
};

class CrimeDataPack{
    /* crime data pack to perform bulk operations */
public:
    CrimeDataPack(){}

    /* operator overload for array like access */
    CrimeData operator[](unsigned long i);

    /* function for adding new values to data pack*/
    void append(CrimeData);
    /* gini impurity of this object */
    double gini_impurity();
    bool is_empty(){ return data_.empty(); }
    unsigned long size(){ return data_.size(); }

    /* function that counts labels and their occurences */
    unordered_map<string,int> count_labels();
    /* partition function that seperates data_ with given question */
    double_cdp partition(Question);
    /* function that returns every unique feature in data_ */
    triple_set unique_vals();
    /* utility printing function */
    string results();


private:
    vector<CrimeData> data_;

    /* utility functions */
    friend tuple<double,double> test_tree(DecisionTree&);
    friend class boost::serialization::access;
    template<class A>
    void serialize(A& ar, const unsigned int version){
        ar & data_;
    }};

}

#endif // CRIME_DATA_H
