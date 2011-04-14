#include "../../src/date.h"
#include <gtest/gtest.h>

/* Date *//*{{{*/

/* Should test format: 2001-01-01 12.13 | 2001-01-01 godz 12.13 */
TEST(Date, parseDate_HandlesFullISOLikeDate) { 
  Date d1("2010-01-01 12:13");
  EXPECT_STREQ(d1.toString().c_str(), "2010-01-01T12:13:00");
}

/* Should test format: 01-01-2001 12.13 | 01-01-2001 godz 12.13 */
TEST(Date, parseDate_HandlesFullPolishDate) {
  Date d1("01-01-2001 12:13");
  EXPECT_STREQ(d1.toString().c_str(), "2001-01-01T12:13:00");

  Date d2("01-01-2001 godz 12.13");
  EXPECT_STREQ(d2.toString().c_str(), "2001-01-01T12:13:00");
}

/* Should test format: 2001-01-01 */
TEST(Date, parseDate_HandlesISOLikeDate) {
  Date d1("2010-01-01");
  time_duration td(hours(second_clock::local_time().time_of_day().hours()) + minutes(second_clock::local_time().time_of_day().minutes()));
  string s("2010-01-01T"); s += to_simple_string(td);
  EXPECT_STREQ(d1.toString().c_str(), s.c_str());
}

/* Should test format: 01-01-2001 */
TEST(Date, parseDate_HandlesPolishDate) {
  Date d1("01-01-2010");
  time_duration td(hours(second_clock::local_time().time_of_day().hours()) + minutes(second_clock::local_time().time_of_day().minutes()));
  string s("2010-01-01T"); s += to_simple_string(td);
  EXPECT_STREQ(d1.toString().c_str(), s.c_str());
}

/* Should test format: 20:15 */
TEST(Date, parseDate_HandlesISOLikeTime) {
  Date d1("12:12");
  date today(day_clock::local_day());
}

/* Should test format: dziś */
TEST(Date, parseDate_HandlesToday) {
  Date d1("today");

  date d(day_clock::local_day());
  time_duration td(hours(second_clock::local_time().time_of_day().hours()) + minutes(second_clock::local_time().time_of_day().minutes()));
  string s(to_iso_extended_string(d) + string("T") + to_simple_string(td));
  EXPECT_STREQ(d1.toString().c_str(), s.c_str());
}

/* Should test format: dziś 8 | dziś 8:12 | dziś 0812 */
TEST(Date, parseDate_HandlesTodayWithTime) {
  Date d0("today");
  Date d1("today 8");
  Date d2("today 8:12");
  Date d3("today 0812");

  date d(day_clock::local_day());
  time_duration td(hours(second_clock::local_time().time_of_day().hours()) + minutes(second_clock::local_time().time_of_day().minutes()));
  string todayDate(to_iso_extended_string(d));

  string s(todayDate + string("T") + to_simple_string(td));
  EXPECT_STREQ(d0.toString().c_str(), s.c_str());
  
  time_duration td2(hours(8) + minutes(second_clock::local_time().time_of_day().minutes()));
  s = (todayDate + string("T") + to_simple_string(td2));
  EXPECT_STREQ(d1.toString().c_str(), s.c_str());
  s = (todayDate + string("T") + "08:12:00");
  EXPECT_STREQ(d2.toString().c_str(), s.c_str());
  EXPECT_STREQ(d3.toString().c_str(), s.c_str());
}

/* Should test format: 15 marca */
/* TODO: this test WILL fail in 2011 ;)  */
TEST(Date, parseDate_HandlesDayMonth) {
  Date d1("15 marca");
  time_duration td2(hours(second_clock::local_time().time_of_day().hours()) + minutes(second_clock::local_time().time_of_day().minutes()));
  string s("2010-03-15T" + to_simple_string(td2));
  EXPECT_STREQ(d1.toString().c_str(), s.c_str());
}

/*}}}*/
/* parseWeekDayNameTest *//*{{{*/

TEST(Date, parseWeekDayName) {
  Date d;
  EXPECT_EQ(d.parseWeekDayName("niedziela"), 0);
  EXPECT_EQ(d.parseWeekDayName("poniedziałek"), 1);
  EXPECT_EQ(d.parseWeekDayName("wtorek"), 2);
  EXPECT_EQ(d.parseWeekDayName("środa"), 3);
  EXPECT_EQ(d.parseWeekDayName("czwartek"), 4);
  EXPECT_EQ(d.parseWeekDayName("piątek"), 5);
  EXPECT_EQ(d.parseWeekDayName("sobota"), 6);
}
/*}}}*/
/* parseMonthNameTest *//*{{{*/

TEST(Date, parseMonthName) {
  Date d;
  EXPECT_EQ(d.parseMonthName("styczeń"), 1);
  EXPECT_EQ(d.parseMonthName("luty"), 2);
  EXPECT_EQ(d.parseMonthName("marzec"), 3);
  EXPECT_EQ(d.parseMonthName("kwiecień"), 4);
  EXPECT_EQ(d.parseMonthName("maj"), 5);
  EXPECT_EQ(d.parseMonthName("czerwiec"), 6);
  EXPECT_EQ(d.parseMonthName("lipiec"), 7);
  EXPECT_EQ(d.parseMonthName("sierpień"), 8);
  EXPECT_EQ(d.parseMonthName("wrzesień"), 9);
  EXPECT_EQ(d.parseMonthName("październik"), 10);
  EXPECT_EQ(d.parseMonthName("listopad"), 11);
  EXPECT_EQ(d.parseMonthName("grudzień"), 12);
}
/*}}}*/
/* getYearX test */

TEST(PTIME , PTIMETEST) {
  date d1(2010, 1, 2);
  date d2(2011, 1, 2);
  ptime p1(d1, minutes(5));
  ptime p2(d2, minutes(5));
  EXPECT_TRUE(p1 < p2);
}
