// The next line includes
// and a few lines of adapter code.
#include <boost/icl/interval_map.hpp>
#include <boost/icl/ptime.hpp>
#include <iostream>

using boost::icl::interval;
using boost::posix_time::ptime;
using boost::posix_time::time_from_string;
using boost::icl::operator<<;

// Type set collects the names of party guests. Since std::set is
// a model of the itl's set concept, the concept provides an operator +=
// that performs a set union on overlap of intervals.

void boost_party() {
  std::set<std::string> mary_harry = {"Mary", "Harry"};
  std::set<std::string> diana_susan = {"Diana", "Susan"};
  std::set<std::string> peter = {"Peter"};

  // A party is an interval map that maps time intervals to sets of guests
  boost::icl::interval_map<ptime, std::set<std::string>> party;

  party.add(  // add and element
      std::make_pair(
          interval<ptime>::right_open(time_from_string("2008-05-20 19:30"),
                                      time_from_string("2008-05-20 23:00")),
          mary_harry));

  party +=  // element addition can also be done via operator +=
      std::make_pair(
          interval<ptime>::right_open(time_from_string("2008-05-20 20:10"),
                                      time_from_string("2008-05-21 00:00")),
          diana_susan);

  party += std::make_pair(
      interval<ptime>::right_open(time_from_string("2008-05-20 22:15"),
                                  time_from_string("2008-05-21 00:30")),
      peter);

  std::cout << "----- History of party guests -------------------------\n";

  for (const auto& [when, who] : party) {
    // Who is at the party within the time interval 'when' ?
    std::cout << when << ": " << who << std::endl;
  }
}

int main() {
  std::cout << ">>Interval Container Library: Sample boost_party.cpp <<\n";
  std::cout << "-------------------------------------------------------\n";
  boost_party();
  return 0;
}

// Program output:
/*-----------------------------------------------------------------------------
>>Interval Container Library: Sample boost_party.cpp <<
-------------------------------------------------------
----- History of party guests -------------------------
[2008-May-20 19:30:00, 2008-May-20 20:10:00): Harry Mary
[2008-May-20 20:10:00, 2008-May-20 22:15:00): Diana Harry Mary Susan
[2008-May-20 22:15:00, 2008-May-20 23:00:00): Diana Harry Mary Peter Susan
[2008-May-20 23:00:00, 2008-May-21 00:00:00): Diana Peter Susan
[2008-May-21 00:00:00, 2008-May-21 00:30:00): Peter
-----------------------------------------------------------------------------*/
