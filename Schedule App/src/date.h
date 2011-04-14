#ifndef DateInclude
#define DateInclude

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>
#include <vector>
#include <string>
#include "regex.h"

using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace boost;
using namespace std;

class Date {
  ptime *dateTime;
  public:
    Date(string text);
    Date(date, time_duration);
    Date(int);
    Date();
    string toString(); 
    int stringToInt(const string &str);
    vector<int>& fillRawDateVector(vector<string> &matches);
    void parseDate(string text);
    int parseWeekDayName(string) const; /* converts weekDay to int */
    int parseMonthName(string) const; /* converts month to int */
    Date currentDate() const; /* returns current dateTime */
    Date getHourB() const; 
    Date getHourE() const;
    Date getDayB() const;
    Date getDayE() const;
    Date getWeekB() const; /* returns Date of this weeks Monday */
    Date getWeekE() const; /* returns Date of this weeks Sunday */
    Date getMonthB() const; 
    Date getMonthE() const; 
    Date getYearB() const; /* returns Date of january 1 */
    Date getYearE() const; /* returns Date of last day of december */
    bool operator<(const Date &right) const;
    bool operator>(const Date &right) const;
};

#endif
