# Ticket Office

## Description
Write a program that supports the work of the ticket office selling tickets for connections
Tram. Trams are open from 5:55 to 21:21 (they do not run outside these hours
and passengers do not use calls for more than one day).
The program receives standard input lines in the formats described below.

#### 1 \. Adding a new course to the timetable

`course number time_1 stop_1 time_2 stop_2 ... time_n stop_n`

Course_number is an integer, possibly with additional zeros on the beginning (we ignore them).
Course numbers do not repeat.
Time_x parameters are in normal format, e.g. 7:15 or 19: 01 \.
Times for the next stops are increasing and the course does not go more than once by one bus stop.
Parameters stop_x are the names of the stops - they are non-empty compound strings the letters of the English alphabet and the characters '_' and '^'.

#### 2 \. Adding a ticket to the timetable

`ticket_name price expiry_time`

The parameter tab_name can only contain letters from the English alphabet and spaces and it is a non-empty string.
The price parameter is a number with two decimal places, e.g. 6.23.
The expiration time parameter is a non-zero natural number with no leading zeros - the number of minutes the ticket is valid. For example, a two-minute ticket deleted at 8:00 is valid from 8:00 to 8:01 (first minute from 8:00:00 to 8:00:59 and the other from 8:01:00 to 8:01:59).

####3 \. Question about tickets

`? stop_1 course_number_1 stop_2 course_number_2 ... stop_n course_number_n stop_n + 1`

The traveler wants to buy the cheapest set of tickets covering the given route. It may consist of at most three tickets. The traveler gets on the tram number_course_1 at stop stop_1, at stop_2 it gets off from it and gets on the tram course_number_2 etc., and finally gets off at stop stop_n + 1.
The ticket validity ranges are closed and can be combined as desired, e.g. by driving by tram from 10:10 to 10:29 you can buy two 10-minute tickets - one valid from 10:10 to 10:19, and the second from 10:20 to 10:29.

A set of tickets should be written to the standard output in the following format:

`! nazwa_biletu; nazwa_biletu_2; ...; nazwa_biletu_n`

Passengers don't like waiting at the bus stop and transfers are always on the spot, e.g. the tram arrives at the stop at 15:00 and the next leaves from this stop at 15:00. If you have to wait, we don't offer tickets, only print out to the standard output:

`:-( stop_name_where_ you need to wait`

If it is not possible to buy tickets for the given route, then we write it out for the standard one exit:

`:-|`

If there is more than one set of tickets at a minimum price, then it should be list any of them.

Finally, write the total number of tickets proposed in all answers to the question (third variant of the input format).

## Functional requirements ##

The program reads the data line by line from the standard input. The program analyzes input line by line. The program ignores blank lines. For everyone non-empty line, the program decides whether this line is valid. If the line is not correct, the information it contains is not taken into account for further processing, and the program writes to standard output diagnostic line with error information containing the incorrect command line number and its exact original form. In particular the data line from outside the tram hours set in the content is incorrect.
The lines are numbered from 1. Use the format exactly as stated in the example.

The program will be compiled on the with the command:
```shell script
g++ -Wall -Wextra -O2 -std=c++17 kasa.cc -o kasa
```