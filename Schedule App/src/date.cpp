#include "date.h"

  int Date::stringToInt(const string &str) {/*{{{*/
    int result = atoi(str.c_str());
    if (result)
      return result;
    else
      return -1;
  }/*}}}*/
  vector<int>& Date::fillRawDateVector(vector<string> &matches) {/*{{{*/
    vector<int> *rawDateVector = new vector<int>;

    for(unsigned i=0; i < 6; i++) {
      if (i < matches.size()) 
        rawDateVector->push_back(stringToInt(matches[i]));
      else
        rawDateVector->push_back(-1);
    }

    return (*rawDateVector);
  }/*}}}*/
  int Date::parseWeekDayName(string weekDay) const{ /*{{{*/
    regex sunday("[nN]iedziela");
    regex monday("[pP]oniedziałek");
    regex tuesday("[wW]torek");
    regex wednesday("środa|Środa");
    regex thursday("[cC]zwartek");
    regex friday("[pP]iątek");
    regex saturday("[sS]obota");
    vector<regex> patternVector;
    vector<regex>::iterator it;

    patternVector.push_back(sunday);
    patternVector.push_back(monday);
    patternVector.push_back(tuesday);
    patternVector.push_back(wednesday);
    patternVector.push_back(thursday);
    patternVector.push_back(friday);
    patternVector.push_back(saturday);

    unsigned i;
    for ( it = patternVector.begin(), i = 0; it != patternVector.end(); it++, i++ ) 
      if ( Regex::test(weekDay, *it) )
        return i;
    return -1;
  }/*}}}*/
  int Date::parseMonthName(string month) const{ /*{{{*/
    regex january("[sS]tyczeń|[sS]tycznia");
    regex february("[lL]uty|[lL]utego");
    regex march("[mM]arzec|[mM]arca");
    regex april("[kK]wiecień|[kK]wietnia");
    regex may("[mM]aj|[mM]aja");
    regex june("[cC]zerwiec|[cC]zerwca");
    regex july("[lL]ipiec|[lL]ipca");
    regex august("[sS]ierpień|[sS]ierpnia");
    regex september("[wW]rzesień|[wW]rześnia");
    regex october("[pP]aździernik|[pP]aździernika");
    regex november("[lL]istopad|[lL]istopada");
    regex december("[gG]rudzień|[gG]rudnia");
    vector<regex> patternVector;
    vector<regex>::iterator it;
    patternVector.push_back(january);
    patternVector.push_back(february);
    patternVector.push_back(march);
    patternVector.push_back(april);
    patternVector.push_back(may);
    patternVector.push_back(june);
    patternVector.push_back(july);
    patternVector.push_back(august);
    patternVector.push_back(september);
    patternVector.push_back(october);
    patternVector.push_back(november);
    patternVector.push_back(december);
    unsigned i;
    for ( it = patternVector.begin(), i = 0; it != patternVector.end(); it++, i++ ) 
      if (Regex::test(month, *it))
        return i+1;
    return -1;
  }/*}}}*/
  Date Date::currentDate() const{/*{{{*/
    return Date("today");
  }/*}}}*/
  Date Date::getHourB() const{/*{{{*/
    date today(day_clock::local_day());
    Date d(today, hours(second_clock::local_time().time_of_day().hours()));
    return d;
  }/*}}}*/
  Date Date::getHourE() const{/*{{{*/
    date today(day_clock::local_day());
    Date d(today, hours(second_clock::local_time().time_of_day().hours()) + hours(1));
    return d;
  }/*}}}*/
  Date Date::getDayB() const{/*{{{*/
    date today(day_clock::local_day());
    Date d(today, hours(0));
    return d;
  }/*}}}*/
  Date Date::getDayE() const{/*{{{*/
    date today(day_clock::local_day());
    Date d(today, hours(24));
    return d;
  }/*}}}*/
  Date Date::getWeekB() const{/*{{{*/
    greg_weekday wd(Monday);
    date d(previous_weekday(dateTime->date(), wd));
    return Date(d, hours(0));
  }/*}}}*/
  Date Date::getWeekE() const{/*{{{*/
    greg_weekday wd(Sunday);
    /* weekday starts at 0000 Monday & Ends @ 0000 Monday */
    date d(next_weekday(dateTime->date(), wd) + days(1));
    return Date(d, hours(0));
  }/*}}}*/
  Date Date::getMonthB() const{/*{{{*/
    date d(dateTime->date().year(), dateTime->date().month(), 1);
    return Date(d, minutes(0));
  }/*}}}*/
  Date Date::getMonthE() const{/*{{{*/
    unsigned lastDay = gregorian_calendar::end_of_month_day(dateTime->date().year(), dateTime->date().month());
    date d(dateTime->date().year(), dateTime->date().month(), lastDay);
    return Date(d, minutes(0));
  }/*}}}*/
  Date Date::getYearB() const{/*{{{*/
    date d(dateTime->date().year(), 1, 1);
    Date result(d, minutes(0));
    return result;
  }/*}}}*/
  Date Date::getYearE() const{/*{{{*/
    date d(dateTime->date().year(), 12, 31);
    Date result(d, minutes(0));
    return result;
  }/*}}}*/
  void Date::parseDate(string text) {/*{{{*/
    string patternYear("(\\d{4})");
    string dateSeparator("[\\.\\-\\/]");
    string patternMonth("(\\d{2}|\\d{1})");
    string patternDay("(\\d{2}|\\d{1})");
    string dateTimeSeparator("[\\ ]");
    string patternHour("(\\d{2}|\\d{1})");
    string timeSeparator("[\\:\\.]");
    string patternMinute("(\\d{2}|\\d{1})");
    string optional("?");
    string _or("|");
    string time(patternHour + timeSeparator + patternMinute);
    string patternMonthLiteral("(stycznia|lutego|marca|kwietnia|maja|czerwca|lipca|sierpnia|września|października|listopada|grudnia)");
    // regex dayMonth("\\d{1,2}\\ " + patternMonthLiteral);
    regex dayMonth("(\\d{1,2})\\ " + patternMonthLiteral);
    // regex weekDay("");
    regex dayAfterTomorrow("pojutrze");
    regex todayPattern("(?:dziś|today)(?:\\ )?(?:(\\d{1,2}$)|(\\d{2})(\\d{2}$)|(\\d{1,2})[\\:\\.](\\d{1,2}$))?");
    regex tomorrowPattern("(?:jutro|tomorrow)(?:\\ )?(?:(\\d{1,2}$)|(\\d{2})(\\d{2}$)|(\\d{1,2})[\\:\\.](\\d{1,2}$))?");
    regex fullDateTime(patternYear + dateSeparator + patternMonth + dateSeparator + patternDay + 
                      dateTimeSeparator + "(?:godz\\ )?" +
                      patternHour + timeSeparator + patternMinute);
    regex ISOLikeDate(patternYear + dateSeparator + patternMonth + dateSeparator + patternDay);
    regex ISOLikeTime(patternHour + timeSeparator + patternMinute);
    regex fullDateTimePolish(patternDay + dateSeparator + patternMonth + dateSeparator + patternYear +
                             dateTimeSeparator + "(?:godz\\ )?" +
                             patternHour + timeSeparator + patternMinute);
    regex polishDate(patternDay + dateSeparator + patternMonth + dateSeparator + patternYear);
    regex fullDate("(\\d{4})-(\\d{2})-(\\d{2})T(\\d{2}):(\\d{2}):(\\d{2})");
    int year, month, day, hour, minute;
    year = month = day = hour = minute = -1;
    vector<int> dateVector;
    /* construct date object inserting local time when not given input */
    date today(day_clock::local_day());

    vector<string> matches;
    if (Regex::match(text,fullDateTime, matches)) {
      dateVector = fillRawDateVector(matches);
      year = dateVector[0];
      month = dateVector[1];
      day = dateVector[2];
      hour = dateVector[3];
      minute = dateVector[4];
    }else if (Regex::match(text, fullDate, matches)) {
      dateVector = fillRawDateVector(matches);
      year = dateVector[0];
      month = dateVector[1];
      day = dateVector[2];
      hour = dateVector[3];
      minute = dateVector[4];
    }else if (Regex::match(text, fullDateTimePolish, matches)) {
      dateVector = fillRawDateVector(matches);
      day = dateVector[0];
      month = dateVector[1];
      year = dateVector[2];
      hour = dateVector[3];
      minute = dateVector[4];
    }else if (Regex::match(text, ISOLikeDate, matches)) {
      dateVector = fillRawDateVector(matches);
      year = dateVector[0];
      month = dateVector[1];
      day = dateVector[2];
    }else if (Regex::match(text, polishDate, matches)) {
      dateVector = fillRawDateVector(matches);
      day = dateVector[0];
      month = dateVector[1];
      year = dateVector[2];
    // }else if (Regex::match(text, ISOLikeTime, matches)) {
    //   dateVector = fillRawDateVector(matches);
    //   hour = dateVector[0];
    //   minute = dateVector[1];
    }else if (Regex::match(text, todayPattern, matches)) {
      dateVector = fillRawDateVector(matches);
      hour = dateVector[0];
      minute = dateVector[1];
    }else if (Regex::match(text, tomorrowPattern, matches)) {
      today = today + days(1);
      day = -1;
      dateVector = fillRawDateVector(matches);
      hour = dateVector[0];
      minute = dateVector[1];
    // }else if (Regex::match(text, dayAfterTomorrow, matches)) {
    //   today = today + days(2);
    }else if (Regex::match(text, dayMonth, matches)) {
      day = stringToInt(matches[0]);
      month = parseMonthName(matches[1]);
    }


    year = year != -1 ? (boost::gregorian::greg_year)year : today.year();
    month = month != -1 ? (boost::gregorian::greg_month)month : today.month();
    day = day != -1 ? (boost::gregorian::greg_day)day : today.day();

    date d(year, month, day);

    /* construct time_duration object inserting local time when not given input */
    hour = hour != -1 ? hour : second_clock::local_time().time_of_day().hours();
    minute = minute != -1 ? minute : second_clock::local_time().time_of_day().minutes();

    time_duration td(hour, minute, 0, 0);

    /* construct ptime by adding in hours and minutes */
    dateTime = new ptime(d, td);
  }/*}}}*/
  Date::Date() {/*{{{*/
  }/*}}}*/
  Date::Date(date d, time_duration td) {/*{{{*/
    dateTime = new ptime(d, td);
  }/*}}}*/
  Date::Date(string text) {/*{{{*/
    parseDate(text);
  }/*}}}*/
  string Date::toString() {/*{{{*/
    if (dateTime != NULL)
      return to_iso_extended_string(*dateTime);
    else
      return string("DateTime NULL!!");
  }/*}}}*/
  bool Date::operator<(const Date &right) const {/*{{{*/
    ptime t1, t2;
    t1 = *dateTime;
    t2 = *(right.dateTime);
    return (t1 < t2);
  }/*}}}*/
  bool Date::operator>(const Date &right) const {/*{{{*/
    ptime t1, t2;
    t1 = *dateTime;
    t2 = *(right.dateTime);
    return (t1 > t2);
  }/*}}}*/
