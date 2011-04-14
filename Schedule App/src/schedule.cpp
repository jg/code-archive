/*    schedule.cpp
 *
 *    Simple calendar program 
 *    Juliusz Gonera 2010/03/09
 *
 *    See README for details
 */

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp> 
#include "regex.h"
#include "date.h"

using namespace std;
using namespace boost;

class Event {/*{{{*/
  public:
    Date timeStamp;
    string description;

    Event(string eventDescription, Date date) {/*{{{*/
      this->timeStamp = date;
      this->description = eventDescription;
    }/*}}}*/
    string toString() {/*{{{*/
      string s(description + " " + timeStamp.toString());
      return s;
    }/*}}}*/
};/*}}}*/
class Schedule {/*{{{*/
  public:
  vector<Event> events;

  void saveEvents() {/*{{{*/
    vector<Event>::iterator it;
    ofstream file;

    file.open("schedule.txt");

    for ( it = events.begin(); it != events.end(); it++ ) 
      file << it->description << "||" << it->timeStamp.toString() << endl;

    file.close();
  }/*}}}*/
  void readEvents() {/*{{{*/
    vector<Event>::iterator it;
    ifstream file;

    file.open("schedule.txt", ios::in);
    string line;
    getline(file, line);

    if ( file.is_open() ) {
      while ( !file.eof() ) {
        string description(line.substr(0, line.find("||")));
        string date(line.substr(line.find("||") + 2, line.size() - line.find("||")));

        Date d(date);
        Event e(description, date);
        addEvent(e);

        getline(file, line);
      }
    }

    file.close();
  }/*}}}*/
  void addEvent(Event e) {/*{{{*/
    events.push_back(e);
  }/*}}}*/
  vector<Event> getEvents(Date t1, Date t2) { /* Return a vector of Events with dates satisfying t1 < e.date < t2 *//*{{{*/
    vector<Event>::iterator it;
    vector<Event> results;
    for ( it = events.begin(); it != events.end(); it++ ) 
      if ( ((it->timeStamp) > t1) && ((it->timeStamp) < t2) ) results.push_back(*it); 
      
    return results;
  }/*}}}*/
  void listEvents(Date t1, Date t2) { /* lists all events with dates satisfying t1 < e.date < t2 *//*{{{*/
    vector<Event>::iterator it;
    vector<Event> results = getEvents(t1, t2);

    int i = 1;
    for ( it = results.begin(); it != results.end(); it++, i++ ) 
      cout << "   " << i << ". " << "\"" << it->description << "\" " << it->timeStamp.toString() <<  endl;
    
  }/*}}}*/
  void listEvents() {/*{{{*/
    vector<Event>::iterator it;

    for ( unsigned i=0; i < events.size(); i++ ) 
      cout << "\"" << events[i].description << "\"" << "\t\t" << events[i].timeStamp.toString() << endl;
  }/*}}}*/
  void displayRange(Date t1, Date t2) {/*{{{*/
    cout << "Listing events from: " << t1.toString() << " to " << t2.toString() << endl;
    cout << "---------------------------------------------------------------" << endl;
  }/*}}}*/
  void listCurrentHourEvents() {/*{{{*/
    Date t1("today");
    displayRange(t1.getHourB(), t1.getHourE());
    listEvents(t1.getHourB(), t1.getHourE());
  }/*}}}*/
  void listCurrentDayEvents() {/*{{{*/
    Date t1("today");
    displayRange(t1.getDayB(), t1.getDayE());
    listEvents(t1.getDayB(), t1.getDayE());
  }/*}}}*/
  void listCurrentWeekEvents() {/*{{{*/
    Date t1("today");
    displayRange(t1.getWeekB(), t1.getWeekE());
    listEvents(t1.getWeekB(), t1.getWeekE());
  }/*}}}*/
  void listCurrentMonthEvents() {/*{{{*/
    Date t1("today");
    displayRange(t1.getMonthB(), t1.getMonthE());
    listEvents(t1.getMonthB(), t1.getMonthE());
  }/*}}}*/
  void listCurrentYearEvents() {/*{{{*/
    Date t1("today");
    displayRange(t1.getYearB(), t1.getYearE());
    listEvents(t1.getYearB(), t1.getYearE());
  }/*}}}*/
  bool dispatch(string input) { /* used in mainLoop *//*{{{*/
    regex addEventPattern("^(ae|AddEvent|addevent)\\s(?:\"|\')(.*)(?:\"|\')\\s(.*)$");
    regex listHourlyPattern("listHour");
    regex listDailyPattern("listDay");
    regex listWeeklyPattern("listWeek");
    regex listMonthlyPattern("listMonth");
    regex listYearlyPattern("listYear");
    regex parseDatePattern("parseDate|pd (.*)");
    regex listAllPattern("listAll");
    regex writeToFile("save");
    regex readFromFile("read");

    vector<string> matches;
    if (input == "help" || input == "h") {
      displayHelp();
    }else if (input == "quit" || input == "q" || input == "exit") {
      return true;
    }else if (Regex::match(input, writeToFile, matches)) {
      saveEvents();
    }else if (Regex::match(input, readFromFile, matches)) {
      readEvents();
    }else if (Regex::match(input, addEventPattern, matches)) {
      Date d(matches[2]);
      Event e(matches[1], d);
      addEvent(e);
    }else if (Regex::match(input, listHourlyPattern, matches)) {
      listCurrentHourEvents();
    }else if (Regex::match(input, listDailyPattern, matches)) {
      listCurrentDayEvents();
    }else if (Regex::match(input, listWeeklyPattern, matches)) {
      listCurrentWeekEvents();
    }else if (Regex::match(input, listMonthlyPattern, matches)) {
      listCurrentMonthEvents();
    }else if (Regex::match(input, listYearlyPattern, matches)) {
      listCurrentYearEvents();
    }else if (Regex::match(input, listAllPattern, matches)) {
      listEvents();
    }else if (Regex::match(input, parseDatePattern, matches)) {
      Date d(matches[0]);
      cout << d.toString() << endl;
    }else {
      cout << "Command unrecognized (hit 'h' for help)" << endl;
    }
    return false;
  }/*}}}*/
  void mainLoop() { /* *//*{{{*/
    bool finished = false;
    string input;

    while ( !finished ) {
      if ( isatty(fileno(stdin)) ) {
        cout <<  "%> ";
        getline(cin, input);
      } else {
        getline(cin, input);
        cout << "%> " << input << endl;
      }

      finished = dispatch(input);
    }
  }/*}}}*/
  void displayHelp() {/*{{{*/
    cout << "Schedule commands are: " << endl << endl
         << "   ae \"DESC\" DATETIME             adds event named DESC with DATETIME which can be in one of the following formats:" << endl
         << "                                  15 marca || 20.03.2003 || jutro, for more supported date formats see tests/unit/dateTest.cpp" << endl
         << "   pd DATETIME                    parses a datetime string and prints the results" << endl
         << "   listYear                       lists all events in the current year" << endl
         << "   listMonth                      lists all events in the current month" << endl
         << "   listWeek                       lists all events in the current week" << endl
         << "   listDay                        lists all events in the current day" << endl
         << "   listHour                       lists all events in the current hour" << endl
         << "   save                           save eventlist to file (saves by default to schedule.txt)" << endl
         << "   read                           read eventslist from file (reads by default to schedule.txt)" << endl
         << "   q                              quit" << endl 
         << endl
         << "Program has also a non-interactive mode, see examples in tests/system" << endl;
  }/*}}}*/
};/*}}}*/


int main() {
  Schedule schedule;
  schedule.mainLoop();
}
