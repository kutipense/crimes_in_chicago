#ifndef QUESTION_H
#define QUESTION_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <typedefs.h>

namespace chicago_crime {

using std::string;

struct CrimeData;

class Question{
public:
    Question();
    Question(int, QuestionType);
    Question(LocationType);

    /* function that evaluates logical compare between question vals and cd */
    bool ask(CrimeData& cd);
    /* utility printing function */
    string get_desc();
private:
    /* type of question */
    QuestionType q_type_;
    /* values to compare*/
    int val_;
    LocationType val_l_;

    /* utility funtions */
    friend class boost::serialization::access;
    template<class A>
    void serialize(A& ar, const unsigned int version){
        ar & val_ & q_type_ & val_l_;
    }
};
}


#endif // QUESTION_H
