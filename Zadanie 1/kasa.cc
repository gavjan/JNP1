#include <iostream>
#include <bits/stdc++.h>
#include <regex>
#include <utility>

using ull = unsigned long long;
using ui = unsigned int;
const ull MAXIMUM_DP_VALUE = std::numeric_limits<ull>::infinity() - 1;
const int MAXIMUM_NO_OF_ITEMS = 3;
const int MAXIMUM_VALIDITY = 1000;
const int TWO_NUMBERS_AFTER_DEC = 2;
const int PAST_TEN_HOUR_LENGTH = 5;
const int BASE_TEN = 10;
const int MIN_TIME_LENGTH = 4; // H:MM
const int MAX_TIME_LENGTH = 5; // HH:MM

/**
 * Para reprezentująca godzinę: <godzina, minuty>
 */
using hourTime = std::pair<int, int>;
/**
 * Stałe oznaczające godzinę otwarcia i zamknięcia tramwajowni
 */
const hourTime openingHour ((int)5, (int)55);
const hourTime closingHour ((int)21, (int)21);
/*
 * Para reprezentująca informację o cenie wycieczki: <cena zestawu biletów, multizbiór nazw biletów>
 */
using pricePair = std::pair<ull, std::multiset<std::string>>;
using priceArray = std::vector<std::vector<pricePair>>;
/**
 * Mapa reprezentująca informację o liniach tramwajowych:
 * Kluczami są numery lini tramwajowych
 * Wartościami są mapy reprezentujące linię tramwajową (kluczami są nazwy przystanków, a wartościami godziny odjazdów
 */
using tramMap = std::map<long long, std::map<std::string, hourTime>>;
/**
 * Mapa reprezentująca informację o dostępnych biletach, ich czasie ważności i cenie:
 * Kluczami są nazwy biletów
 * Wartościami są pary <cena, czas ważności>
 */
using ticketMap = std::map<std::string, std::pair<ull, long long>>;

/**
 * Typ reprezentujący status wyniku, wyszukiwania najtańszego zestawu biletów mającego pokryć całą trasę
 */
enum JourneyMode {
    ticketsAvailable = 0,
    waitOnStop,
    ticketsNotAvailable,
    wrongRoute
};

/**
 * Aktualizuje ceny wycieczek przy uwzględnieniu nowo dodanego biletu
 * @param ticketName - napis oznaczający nazwę biletu
 * @param tickets - referencja na mapę zawierającą informacje o biletach
 * @param tripPrices - referencja na tablicę zawierającą ceny wycieczek określonej długości
 */
void updateTripPrices(const std::string& ticketName, ticketMap& tickets, priceArray& tripPrices) {
    ull price;
    int validity;
    std::tie(price, validity) = tickets[ticketName];

    // Bilet o ważności dłuższej niż możliwa jest praktycznie taki sam jak bilet o ważności największej, jaka jest możliwa
    if (validity > MAXIMUM_VALIDITY)
        validity = MAXIMUM_VALIDITY;

    for (int i = 1; i <= MAXIMUM_NO_OF_ITEMS; i++) {
        for (int j = (i == 1 ? validity : MAXIMUM_VALIDITY); j >= 1; j--) {
            if (j < MAXIMUM_VALIDITY && tripPrices[i][j].first > tripPrices[i][j+1].first) {
                tripPrices[i][j] = tripPrices[i][j+1];
            }

            if (j >= validity &&
                tripPrices[i-1][j-validity].first != MAXIMUM_DP_VALUE &&
                tripPrices[i][j].first > price + tripPrices[i-1][j-validity].first) {
                std::multiset<std::string> temp (tripPrices[i-1][j-validity].second);

                temp.emplace(ticketName);

                tripPrices[i][j] = std::make_pair(price + tripPrices[i-1][j-validity].first, temp);
            }
        }
    }
}

/**
 * Oblicze różnicę minut między dwiema poprawnymi parami reprezentującymi godziny
 * @param hour1 - Pierwsza godzina
 * @param hour2 - Druga godzina
 * @return Liczba minut zawartych w przedziale (domkniętym) między pierwszą a drugą godziną
 */
int minuteDifference(const hourTime& hour1, const hourTime& hour2) {
    int h1 = hour1.first, m1 = hour1.second, h2 = hour2.first, m2 = hour2.second;

    return h1*60 + m1 - h2*60 - m2 + 1;
}

/**
 * Inicjalizuję tablicę cen dla wycieczek określonej długości przy zakupie określonej liczby biletów
 * @param tripPrices - referencja na wektor wektorów par cenowych, która będzie służyć do przechowywania informacji
 *                      o cenie wycieczek
 */
void initTripPrices(priceArray& tripPrices) {
    for (int i = 0; i <= MAXIMUM_NO_OF_ITEMS; i++)
        tripPrices.push_back(std::vector<pricePair>(
                MAXIMUM_VALIDITY + 1,
                std::make_pair(MAXIMUM_DP_VALUE, std::multiset<std::string>())
                             )
        );

    for (int i = 0; i <= MAXIMUM_VALIDITY; i++)
        tripPrices[0][i].first = 0;
}

/**
 * Sprawdza, czy para liczb całkowitych jest poprawną reprezentacją godziny przyjazdu/odjazdu tramwaju
 * @param hour - para reprezentująca godzinę
 * @return True/false w zależności od tego, czy para reprezentuje poprawną godzinę i mieści się w zakresie czasu działa-
 *          nia lini tramwajowych
 */
bool isCorrectHour(hourTime hour) {
    return (hour.first >= 0 && hour.first < 24) &&
           (hour.second >= 0 && hour.second < 60) &&
           hour >= openingHour && hour <= closingHour;
}

/**
 * Sprawdza, czy podany numer jest w rzeczywistości istniejącą linią tramwajową
 * @param id - Badany numer
 * @param tramLines - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @return True/false w zależności od tego, czy istnieje linia tramwajowa o podanym numerze
 */
bool isTramLine(long long id, const tramMap& tramLines) {
    return id > 0 && !tramLines.empty() && tramLines.find(id) != tramLines.end();
}

/**
 * Sprawdza, czy istnieje bilet o podanej nazwie
 * @param ticketName - Niepusty napis oznaczający nazwę biletu
 * @param tickets - referencja na mapę zawierającą informacje o biletach
 * @return True/false w zależności od tego, czy istnieje bilet o podanej nazwie
 */
bool isTicket(const std::string& ticketName, const ticketMap& tickets) {
    return !tickets.empty() && tickets.find(ticketName) != tickets.end();
}

/**
 * Sprawdza, czy opis lini tramwajowej jest poprawny
 * @param stops - Wektor par <godzina, nazwa_przystanka> opisujący opis lini tramwajowej od przystanku początkowego
 *                 do końcowego
 * @return True/false w zależności od tego, czy nazwy przystanków są poprawne oraz czy godziny dojazdów są rosnące
 */
bool isStopsCorrect(const std::vector< std::pair< std::pair< int, int >, std::string > >& stops) {
    if (stops.empty())
        return false;

    hourTime lastHour = std::make_pair<int, int>(0, 0);

    std::regex correctStop("[a-zA-Z_^]+");

    std::map<std::string, bool> counter;

    for (auto & [hour, name] : stops) {
        if (!isCorrectHour(hour) || lastHour >= hour || !std::regex_match(name, correctStop))
            return false;
        else {
            if (counter[name]) {
                counter.clear();

                return false;
            }
            else
                counter[name] = false;
        }

        lastHour = hour;
    }

    counter.clear();
    return true;
}

/**
 * Dodaje nową linię tramwajową
 * @param id    - Nr przystanku (liczba nieujemna)
 * @param stops - Wektor par niepustych napisów w formacie
 *                < <godzina_odjazdu, minuta_odjazdu>, nazwa_przystanku>, np. < <9, 17>, "przystanekA"> oznacza, że
 *                linia o numerze id odjeżdża z "przystanekA" o godzinie 9:17
 * @param tramLines - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @return True/false w zależności od tego, czy udało się dodać linię tramwajową
 *
 * Zwraca false, jeśli została wcześniej dodana linia o tym samym numerze, czas odjazdu jest nieprawidłowy
 * (lub wykracza poza zakres czasu, w którym działają tramwaje) lub istnieją dwa przystanki o tym samym czasie odjazdu
 */
bool addTramLine(long long id,
                 const std::vector< std::pair< std::pair< int, int >, std::string > >& stops,
                 tramMap& tramLines) {
    // Jeśli linia tramwajowa o danym numerze istnieje
    if (isTramLine(id, tramLines) || !isStopsCorrect(stops))
        return false;

    // Sprawdza, czy udało się dodać element do mapy
    bool isSuccessful = false;

    std::tie(std::ignore, isSuccessful) = tramLines.emplace(id, std::map<std::string, std::pair< int, int >>());

    if (!isSuccessful)
        return false;

    for (auto & stop : stops) {
        std::tie(std::ignore, isSuccessful) = tramLines[id].emplace(stop.second, stop.first);

        if (!isSuccessful) {
            tramLines[id].clear();
            tramLines.erase(id);

            return false;
        }
    }

    return true;
}

/**
 * Sprawdza, czy daną linią można przejechać
 * @param firstStop - Nazwa przystanku startowego
 * @param lastStop  - Nazwa przystanku końcowego
 * @param lineId    - Nr linii tramwajowej
 * @param tramLines - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @return True/false w zależności od tego, czy trasą o numerze lineId można przejechać z przystanku firstStop do
 *         przystanku lastStop
 *
 * Zwraca false, jeśli linia nie przejeżdża przez co najmniej jeden z przystanków, co najmniej jeden z przystanków
 * o podanej nazwie nie istnieje, linia tramwajowa o podanym numerze nie istnieje lub linia przejeżdża wcześniej przez
 * lastStop niż przez firstStop
 */
bool isRouteAvailable(const std::string& firstStop, const std::string& lastStop, long long lineId, tramMap& tramLines) {
    return firstStop != lastStop &&
           isTramLine(lineId, tramLines) &&
           tramLines[lineId].find(firstStop) != tramLines[lineId].end() &&
           tramLines[lineId].find(lastStop) != tramLines[lineId].end() &&
           tramLines[lineId][firstStop] < tramLines[lineId][lastStop];
}

/**
 * Dodaje nowy rodzaj biletu
 * @param ticketName    - Nazwa biletu (niepusty napis) składający się z liter łacińskich i spacji
 * @param price         - Cena biletu w groszach
 * @param validity      - Czas ważności biletu (niezerowa liczba naturalna)
 * @param tickets       - referencja na mapę zawierającą informacje o biletach
 * @param tripPrices    - referencja na wektor wektorów par cenowych, która będzie służyć do przechowywania informacji
 *                      o cenie wycieczek
 * @return True/false w zależności od tego, czy udało się dodać linię tramwajową.
 *
 * Zwraca false, jeśli nazwa biletu jest pustym napisem, cena biletu ma więcej niż 2 miejsca dziesiętne, czas ważności
 * biletu wynosi 0, lub został już wcześniej dodany taki sam bilet.
 */
bool addTicket(const std::string& ticketName, ull price, long long validity, ticketMap& tickets, priceArray& tripPrices) {
    if (isTicket(ticketName, tickets))
        return false;

    if (tickets.empty())
        initTripPrices(tripPrices);

    bool isSuccessful = false;

    std::tie(std::ignore, isSuccessful) = tickets.emplace(ticketName, std::make_pair(price, validity));

    if (!isSuccessful)
        return false;

    updateTripPrices(ticketName, tickets, tripPrices);

    return true;
}

/**
 * Znajduje najtańszy zestaw biletów, który pokrywa całą trasę
 * @param routeSegment  - Wektor odcinków trasy, czyli par <nazwa_przystanku, nr_kursu> (np. <"przystanekA", 1> oznacza,
 *                        że chcemy pojechać z "przystanekA" trasą o numerze 1) w kolejności kolejnych przystanków na
 *                        trasie
 * @param lastStop      - Nazwa ostatniego przystanku na trasie
 * @param tramLines     - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @param tripPrices    - Referencja na wektor wektorów par cenowych, która będzie służyć do przechowywania informacji
 *                      o cenie wycieczek
 * @return Para w postaci <typ_wyniku, wynik> - typ_wyniku - składowa journeyMode, wynik - zbiór napisów
 *         reprezentujących wynik
 *
 * W zależności od typu wyniku (składowa typu wyliczeniowego journeyMode):
 * - typ_wyniku == ticketsAvailable - Wynik jest zbiorem nazw najtańszych biletów, które pokrywają całą trasę
 * - typ_wyniku == waitOnStop - Trasa zawiera przystanek, na którym trzeba czekać. Wynik jest zbiorem jednoelementowym
 *                              zawierającym nazwę pierwszego takiego przystanka.
 * - typ_wyniku == ticketsNotAvailable - Nie istnieje odpowiedni zestaw biletów, który pokrywałby całą trasę. Wynikiem
 *                                       jest zbiór pusty.
 * - typ_wyniku == wrongRoute - Zły format drogi (np. dany przystanek nie istnieje)
 */
std::pair< JourneyMode, std::multiset< std::string > >
findJourneyTickets(std::vector< std::pair< std::string, long long> > routeSegment,
                   const std::string& lastStop,
                   tramMap& tramLines,
                   priceArray& tripPrices) {

    std::string previousStop = std::string();
    long long previousLine = -1;
    long long lastLine = -1;

    // Dodajemy strażnika na końcu drogi
    routeSegment.emplace_back(std::make_pair(lastStop, -1));

    // Sprawdzamy, czy format drogi jest dobry
    for (auto s = routeSegment.begin(); s != routeSegment.end(); s++) {
        std::string stopName = s->first;
        long long lineId = s->second;

        if (s != routeSegment.begin()) {
            // Zły format drogi
            if (!isRouteAvailable(previousStop, stopName, previousLine, tramLines)
                || (lineId != -1 && tramLines[previousLine][stopName] > tramLines[lineId][stopName])) {
                return std::make_pair(wrongRoute, std::multiset<std::string>());
            }

            if (lineId == -1)
                lastLine = previousLine;
        }

        previousStop = stopName;
        previousLine = lineId;
    }

    // Sprawdzamy, czy musimy czekać na przystanku
    for (auto s = routeSegment.begin(); s != routeSegment.end(); s++) {
        std::string stopName = s->first;
        long long lineId = s->second;

        if (s != routeSegment.begin()) {
            if (lineId != -1 &&
            tramLines[previousLine][stopName] < tramLines[lineId][stopName]) {
                return std::make_pair(waitOnStop, std::multiset<std::string>({ stopName }));
            }

            if (lineId == -1)
                lastLine = previousLine;
        }

        previousStop = stopName;
        previousLine = lineId;
    }

    // Odtąd wiemy, że droga jest w prawidłowym formacie i nie musimy czekać na żadnym przystanku
    // Nie zainicjowano tablicy z cenami, więc nie dodano żadnego biletu
    if (tripPrices.empty())
        return std::make_pair(ticketsNotAvailable, std::multiset<std::string>());

    auto firstStop = routeSegment.begin();
    int validity = minuteDifference(tramLines[lastLine][lastStop], tramLines[firstStop->second][firstStop->first]);

    // Nie istnieje odpowiedni zestaw biletów
    if (validity > MAXIMUM_VALIDITY) {
        return std::make_pair(ticketsNotAvailable, std::multiset<std::string>());
    }
    else {
        auto result = tripPrices[1][validity];

        for (int i = 2; i <= MAXIMUM_NO_OF_ITEMS; i++) {
            if (result.first > tripPrices[i][validity].first) {
                result = tripPrices[i][validity];
            }
        }

        if (result.first == MAXIMUM_DP_VALUE)
            return std::make_pair(ticketsNotAvailable, std::multiset<std::string>());
        else
            return std::make_pair(ticketsAvailable, result.second);
    }
}

/**
 * Wczytuje wszystkie pozostałe znaki wiersza
 */
void reachEnd() {
    while ((getchar()) != '\n') {}
}
/**
 * Wczytuje pojedynczy znak z wejścia i umieszcza go z powrotem
 * @param lineNum - numer wiersza
 * @return Wczytywany znak
 */
int fpeek() {
    int c;
    c=getc(stdin);
    ungetc(c, stdin);
    return c;
}
/**
 * Wypisuje że wystąpił błąd
 * @param lineNum - numer wiersza
 * @param line - polecenie
 */
void err(int lineNum, const std::string& line) {
    fprintf(stderr, "Error in line %d: %s\n", lineNum,line.c_str());
}
/**
 * Sprawdza czy string jest liczbą
 * @param s - string do sprawdzenia
 * @return True/false w zależności od tego, czy s jest liczbą w porawnej postaci
 */
bool isNumeric(std::string s) {
    for (unsigned long i=0;i<s.size(); i++)
        if (!('0'<=s[i] && s[i]<='9'))
            return false;
    return true;
}
/**
 * Konwertuje cenę od string na float
 * @param s - String dla konwesji
 * @param correct - czy udało się konwertować
 * @return konwertowana liczba
 */
ull getPrice(std::string s, bool& correct) {
    if(s[0]=='.') {
        correct = false;
        return -1;
    }

    ull price = 0;
    ull pow = 1;
    int length = s.size(), i, dotPos=-1;
    for (i = length-1; i >= 0; i--) {
        if(s[i] == '.') {
            if(dotPos != -1) {
                correct = false;
                return -1;
            }

            dotPos = i;
        }
        if (!('0' <= s[i] && s[i] <= '9') && s[i] != '.') {
            correct = false;
            return -1;
        }
        else if ('0' <= s[i] && s[i] <= '9') {
            price += (s[i] - '0') * pow;
            pow *= 10;
        }
    }
    if((s[0]=='0' && s[1]!='.') || dotPos == -1 || length - 1 - dotPos != TWO_NUMBERS_AFTER_DEC) {
        correct = false;
        return -1;
    }

    correct = true;
    return price;
}
/**
 * Sprawdza czy char jest lietrą angielskiego alfabetu
 * @param c - znak
 * @return True/false w zależności od tego, czy c jest lietrą angielskiego alfabetu
 */
bool isLetter(int c) {
    return ('a'<=c && c<='z') || ('A'<=c && c<='Z');
}
/**
 * Sprawdza czy godzina jest w poprawnej postaci
 * @param time - Godzina
 * @return True/false w zależności od tego, czy godzina jest w poprawnej postaci
 */
bool isCorrectTime(std::string time) {
    if(time.size()!=MIN_TIME_LENGTH && time.size()!=MAX_TIME_LENGTH)
        return false;
    if(time[time.size()-3]!=':') return false;
    if(time.size()==PAST_TEN_HOUR_LENGTH && time[0]=='0') return false;

    for(unsigned long i=0;i<time.size();i++) {
        if (!('0'<=time[i] && time[i]<='9') && i!=time.size()-3)
            return false;
    }
    return true;
}
/**
 * Sprawdza czy nazwa przystanku jest poprawna
 * @param line - Nazwa do sprawdzenia
 * @return True/false w zależności od tego, czy nazwa przystanku jest poprawna
 */
bool isCorrectName(std::string line) {
    for(unsigned long i=0;i<line.size();i++)
        if(!isLetter(line[i]) && line[i]!='^' && line[i]!='_')
            return false;
    return true;
}
/**
 * Sprawdza czy nazwa biletu jest poprawna
 * @param line - Nazwa do sprawdzenia
 * @return True/false w zależności od tego, czy nazwa biletu jest poprawna
 */
bool isCorrectTicketName(std::string line) {
    for(unsigned long i=0;i<line.size();i++)
        if(!isLetter(line[i]) && line[i] != ' ')
            return false;
    return true;
}
/**
 * Konwertuje string na liczbę bez wyjątków
 * @param line - String dla konwesji
 * @return konwertowana liczba
 */
int safeConvertToInt(const std::string& hourStr) {
    int num=-1;
    try{num=stoi(hourStr,nullptr,BASE_TEN);}
    catch (...){num=-1;}
    return num;
}
/**
 * Dzieli string, spacją jako separatorem
 * @param line - wiersz polecenia
 * @return vector<string> zawierające podzielone stringi
 */
std::vector<std::string> splitWithSpaces(const std::string& line) {
    std::vector<std::string> tokens;
    unsigned long i = 0, last = 0;
    while ((i = line.find(' ', last)) != std::string::npos) {
        tokens.push_back(line.substr(last, i-last));
        last = i+1;
    }

    tokens.push_back(line.substr(last, std::string::npos));

    return tokens;
}
/**
 * Pobiera i wyczytuje dane wejściowe dla pytania o bilety
 * @param lineNum - numer wiersza
 * @param line - wiersz polecenia
 * @param tramLines - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @param tripPrices - referencja na wektor wektorów par cenowych, która będzie służyć do przechowywania informacji
 *                      o cenie wycieczek
 * @param tickets - referencja na mapę zawierającą informacje o biletach
 * @return True/false w zależności od tego, czy udało się bezbłędnie wykonać polecenie
 *
 */
bool doQuestionMark(int lineNum, std::string line, tramMap& tramLines, priceArray& tripPrices, unsigned int& ticketsSold) {
    if(line.back()==' ' || line.find("  ") != std::string::npos)
        return false;
    std::vector<std::string> tokens=splitWithSpaces(line);
    if(tokens.size()<4) return false;
    if((tokens.size()-2)%2) return false;

    std::vector< std::pair<std::string, long long> > czesci;

    char* pEnd;
    for(unsigned long i=1;i<=tokens.size()-3;i+=2) {
        if(!isCorrectName(tokens[i])) return false;
        if(!isNumeric(tokens[i+1])) return false;

        long long int tramLineId=strtoll(tokens[i + 1].c_str(), &pEnd, BASE_TEN);

        czesci.emplace_back(make_pair(tokens[i], tramLineId));
    }

    auto [status, result] = findJourneyTickets(czesci, tokens[tokens.size()-1], tramLines, tripPrices);


    if (status == ticketsAvailable) {
        std::cout << "! ";

        for (auto iterator = result.begin(); iterator != result.end(); iterator++) {
            // Nazwy biletów
            if (iterator != result.begin())
                std::cout << "; ";
            std::cout << *iterator;
        }
        std::cout << std::endl;

        ticketsSold += result.size();
    }
    else if (status == waitOnStop) {
        std::cout << ":-( ";
        for (auto &s: result) {
            // Nazwa przystanku, na którym czekamy
            std::cout << s;
        }
        std::cout << std::endl;
    }
    else if (status == ticketsNotAvailable) {
        std::cout << ":-|" << std::endl;
    }
    else {
        err(lineNum, line);
    }

    return true;
}
/**
 * Pobiera i wyczytuje dane wejściowe dla dodania nowegu kursu do rozkładu
 * @param line - wiersz polecenia
 * @param tramLines - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @return True/false w zależności od tego, czy udało się bezbłędnie wykonać polecenie
 */
bool doTramLine(std::string line, tramMap& tramLines) {
    if(line.back()==' ' || line.find("  ") != std::string::npos)
        return false;

    std::vector<std::string> tokens=splitWithSpaces(line);

    if(!isNumeric(tokens[0])) return false;
    if((tokens.size()-1)%2) return false;

    char* pEnd;
    long long int lineNum=strtoll(tokens[0].c_str(),&pEnd,BASE_TEN);
    std::vector< std::pair< std::pair<int, int>, std::string> > przystanki;
    int hour, minute;
    for(unsigned long i=1;i<tokens.size();i+=2) {

        if(!isCorrectTime(tokens[i])) return false;
        if(!isCorrectName(tokens[i+1])) return false;
        std::string hourStr = tokens[i].substr(0, tokens[i].find(':'));
        std::string minuteStr = tokens[i].substr(tokens[i].find(':')+1, tokens[i].size()-1);
        hour=safeConvertToInt(hourStr);
        minute=safeConvertToInt(minuteStr);
        if(hour==-1 || minute==-1) return false;

        przystanki.emplace_back(std::make_pair(hour, minute), tokens[i+1]);

    }

    return addTramLine(lineNum, przystanki, tramLines);
}
/**
 * Pobiera i wyczytuje dane wejściowe dla dodania biletu do rozkładu
 * @param line - wiersz polecenia
 * @param tramLines - Referencja na mapę trzymającą informację o liniach tramwajowych
 * @return True/false w zależności od tego, czy udało się bezbłędnie wykonać polecenie
 */
bool doTicket(std::string line, priceArray& tripPrices, ticketMap& tickets) {
    if(line.back()==' ')
        return false;
    int first=-1, second=-1;
    for(int i=line.size()-1;i>=0;i--) {
        if(line[i]==' ') {
            if(first==-1)
                first=i;
            else if(second==-1) {
                second=i;
                break;
            }
        }
    }
    if(first==-1 || second==-1) return false;
    std::string validTimeStr=line.substr(first+1);
    std::string priceStr=line.substr(second+1,first-second-1);
    std::string ticketName=line.substr(0,second);
    if(ticketName.empty()) return false;

    if(!isNumeric(validTimeStr) || validTimeStr[0]=='0') return false;

    bool correct = false;
    ull price = getPrice(priceStr, correct);

    if (!correct)
        return false;
    if (!isCorrectTicketName(ticketName))
        return false;

    int validTime=safeConvertToInt(validTimeStr);
    if (validTime == -1)
        return false;

    return addTicket(ticketName, price, validTime, tickets, tripPrices);
}

int main() {
    tramMap tramLines{};
    priceArray tripPrices;
    ticketMap tickets{};
    unsigned int ticketsSold = 0;

    int lineNum=1, firstChar;
    bool valid=true;
    std::string line;
    while(true) {
        firstChar=fpeek();
        if(firstChar==EOF) {break;}
        if(firstChar=='\n') {
            lineNum++;
            reachEnd();
        }
        else if(firstChar=='?') {
            getline(std::cin,line);
            valid=doQuestionMark(lineNum, line, tramLines, tripPrices, ticketsSold);
            if(!valid)
                err(lineNum, line);
            lineNum++;
        }
        else if('0'<=firstChar && firstChar<='9') {
            getline(std::cin,line);
            valid=doTramLine(line, tramLines);
            if(!valid)
                err(lineNum, line);
            lineNum++;
        }
        else if(isLetter(firstChar) || firstChar == ' ') {
            getline(std::cin,line);
            valid=doTicket(line, tripPrices, tickets);
            if(!valid)
                err(lineNum, line);
            lineNum++;
        }
        else {
            getline(std::cin,line);
            err(lineNum, line);
            lineNum++;
        }
    }

    printf("%u\n", ticketsSold);

    return 0;
}