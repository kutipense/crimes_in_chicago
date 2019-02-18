#include<question.h>
#include<crime_data.h>

namespace chicago_crime {

Question::Question(){}

Question::Question(int val, QuestionType q_type) :
    q_type_(q_type), val_(val)
{}

Question::Question(LocationType val) :  q_type_(LOCATION), val_l_(val) {}

bool Question::ask(CrimeData& cd){
    /* comsider diffrent types and use correct
     * logical operation
     */
    switch (q_type_) {
    case TIME: return val_ >= cd.hour;
    case DISTRICT: return val_ >= cd.district;
    case LOCATION: return cd.exterior == val_l_;
    }
    return true;
}

string Question::get_desc(){
    /* utility function */
    return q_type_ == LOCATION
            ? string("if $x == ") + (val_l_ == EXTERIOR ? "exterior" : "interior")
            : string("if $x <= ") + std::to_string(val_)
              + (q_type_ == DISTRICT ? " (district)" : " (hour)");
}

}



