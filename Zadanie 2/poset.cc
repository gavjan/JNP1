#include <iostream>
#include <map>
#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include <functional>

#include "poset.h"
using namespace std;

namespace {
    using Comparator = function<bool(const unique_ptr<string>&, const unique_ptr<string>&)>;
    using Relations = map<unsigned long, unordered_map<string, set<const unique_ptr<string>*> >[2] >;
    using Values = map<unsigned long, set<unique_ptr<string>, Comparator> >;

    enum ERRORS {
        NULLVALUE1 = 1 << 0,
        NULLVALUE2 = 1 << 1,
        POSET_EXISTS = 1 << 2,
        VALUE1_IN_POSET = 1 << 3,
        VALUE2_IN_POSET = 1 << 4,
        TEST_V1_V2 = 1 << 5,
        TEST_V2_V1 = 1 << 6
    };

    Comparator& cmp() {
        static Comparator cmp = [](const unique_ptr<string>& a, const unique_ptr<string>& b) {
            return *a < *b;
        }; 
        return cmp;
    } 

    Values& values() {
        static Values ans = Values();
        return ans; 
    }

    Relations& relations() {
        static Relations ans = Relations();
        return ans;
    }

#ifndef NDEBUG
    void initCerr() {
        static ios_base::Init init;
    }
#endif

    void writeErr(string str, bool endline = true) {
#ifndef NDEBUG
        initCerr();
        cerr << str << ((endline) ? "\n" : "");
#else
    // Supressing the unused variable warnings
    (void)str;
    (void)endline;
#endif
    }

    inline bool doesExist(unsigned long id) {
        return values().find(id) != values().end();
    }

    inline bool isInPoset(unsigned long id, string value) {
        return values()[id].find(make_unique<string>(value)) != values()[id].end(); 
    }

    inline const unique_ptr<string>& getRef(unsigned long id, string value) {
        return *values()[id].find(make_unique<string>(value));
    }

    bool DFS(unsigned long id, const unique_ptr<string>& value1, const unique_ptr<string>& value2, set<const unique_ptr<string>*>& visited) {
        visited.insert(&value1);
        if(value1 == value2)
            return true;

        for(auto greater : relations()[id][1][*value1]) {
            if(visited.count(greater) != 0)
                continue;

            if(DFS(id, *greater, value2, visited))
                return true;
        }

        return false;
    }

    inline string element(string value) {
        return "\"" + value + "\"";
    }

    inline string relation(string value1, string value2) {
        return "(" + element(value1) + ", " + element(value2) + ")";
    }

    bool checkForErrors(string functionName, unsigned long id, char const* value1, char const* value2, unsigned long errorsToCheck) {
        set<const unique_ptr<string>*> visited = set<const unique_ptr<string>*>();
        bool result = false;

        if((errorsToCheck & POSET_EXISTS) && !doesExist(id)) {
            writeErr(functionName + ": poset " + to_string(id) + " does not exist"); 
            return true;
        }

        if((errorsToCheck & NULLVALUE1) && value1 == NULL) {
            writeErr(functionName + ": invalid value", false); 
            if(functionName != "poset_insert" && functionName != "poset_remove")
                writeErr("1", false); 
            writeErr(" (NULL)"); 
            result = true;
        }

        if((errorsToCheck & NULLVALUE2) && value2 == NULL) {
            writeErr(functionName + ": invalid value2 (NULL)");
            return true;
        }

        if(result)
            return true;
        
        if(((errorsToCheck & VALUE1_IN_POSET) && !isInPoset(id, value1))
           || ((errorsToCheck & VALUE2_IN_POSET) && !isInPoset(id, value2))) {
            
            if(value2 != NULL)
                writeErr(functionName + ": poset " + to_string(id) + ", element " + element(value1) + " or " + element(value2) + " does not exist"); 
            else
                writeErr(functionName + ": poset " + to_string(id) + ", element " + element(value1) + " does not exist");
            return true;
        }

        if((errorsToCheck & TEST_V1_V2) && DFS(id, getRef(id, value1), getRef(id, value2), visited)) { 
            writeErr(functionName + ": poset " + to_string(id) + ", relation " + relation(value1, value2) + " cannot be added"); 
            return true;
        }

        visited.clear();
        if((errorsToCheck & TEST_V2_V1) && DFS(id, getRef(id, value2), getRef(id, value1), visited)) {
            writeErr(functionName + ": poset " + to_string(id) + ", relation " + relation(value1, value2) + " cannot be added"); 
            return true;
        }

        return false;
    }

    void printAFunction(string functionName, int args, unsigned long id = 0, char const* value1 = NULL, char const* value2 = NULL) {
        writeErr(functionName + "(", false); 
        if(args >= 1)
            writeErr(to_string(id), false); 
        if(args >= 2)
            writeErr(", " + ((value1 == NULL) ? "\"NULL\"" : element(value1)), false); 
        if(args >= 3)
            writeErr(", " + ((value2 == NULL) ? "\"NULL\"" : element(value2)), false); 
        writeErr(")"); 
    }
}

namespace jnp1 {
    unsigned long poset_new() {
        printAFunction(__func__, 0);
        static unsigned long maxId = 0;

        values()[maxId] = set<unique_ptr<string>, Comparator>(cmp());
        relations()[maxId][0] = unordered_map<string, set<const unique_ptr<string>* > >();
        relations()[maxId][1] = unordered_map<string, set<const unique_ptr<string>* > >();

        writeErr("poset_new: poset " + to_string(maxId) + " created");
        return maxId++;
    }

    void poset_delete(unsigned long id) {
        printAFunction(__func__, 1, id); 
        if(checkForErrors(__func__, id, NULL, NULL, POSET_EXISTS))
            return;

        relations().erase(id);

        values().erase(id);
        writeErr("poset_delete: poset " + to_string(id) + " deleted");
    }

    size_t poset_size(unsigned long id) {
        printAFunction(__func__, 1, id);
        size_t result = 0;
        if(!checkForErrors(__func__, id, NULL, NULL, POSET_EXISTS)) {
            result = values()[id].size();
            writeErr("poset_size: poset " + to_string(id) + " contains " + to_string(result) + " element(s)"); 
        }
        return result;
    }

    bool poset_insert(unsigned long id, char const* value) {
        printAFunction(__func__, 2, id, value);
        if(checkForErrors(__func__, id, value, NULL, NULLVALUE1 | POSET_EXISTS)) {
            return false;
        }

        if(isInPoset(id, value)) {
            writeErr("poset_insert: poset " + to_string(id) + ", element " + element(value) + " already exists"); 
            return false;
        }

        values()[id].insert(make_unique<string>(value));

        const unique_ptr<string>& strValue = getRef(id, value); 
        relations()[id][0][*strValue].insert(&strValue);
        relations()[id][1][*strValue].insert(&strValue);

        writeErr("poset_insert: poset " + to_string(id) + ", element " + element(value) + " inserted"); 
        return true;
    }

    bool poset_remove(unsigned long id, char const* value) {
        printAFunction(__func__, 2, id, value);
        if(checkForErrors(__func__, id, value, NULL, NULLVALUE1 | POSET_EXISTS | VALUE1_IN_POSET)) {
            return false;
        }

        const unique_ptr<string>& strValue = getRef(id, value);
        for(auto lesser : relations()[id][0][*strValue])
            relations()[id][1][**lesser].erase(&strValue);

        for(auto greater : relations()[id][1][*strValue])
            relations()[id][0][**greater].erase(&strValue);

        for(auto lesser : relations()[id][0][*strValue]) {
            if(*lesser == strValue)
                continue;
            for(auto greater : relations()[id][1][*strValue]) {
                if(*greater == strValue)
                    continue;
                
                relations()[id][1][**lesser].insert(greater);
                relations()[id][0][**greater].insert(lesser);
            }
        }
        relations()[id][0].erase(*strValue);
        relations()[id][1].erase(*strValue);
        values()[id].erase(strValue);

        writeErr("poset_remove: poset " + to_string(id) + ", element " + element(value) + " removed"); 
        return true;
    }

    bool poset_add(unsigned long id, char const* value1, char const* value2) {
        printAFunction(__func__, 3, id, value1, value2);
        if(checkForErrors(__func__, id, value1, value2, NULLVALUE1 | NULLVALUE2 | POSET_EXISTS | VALUE1_IN_POSET
           | VALUE2_IN_POSET | TEST_V1_V2 | TEST_V2_V1)) {
            return false;
        }

        const unique_ptr<string>& strValue1 = getRef(id, value1);
        const unique_ptr<string>& strValue2 = getRef(id, value2);

        relations()[id][1][*strValue1].insert(&strValue2);
        relations()[id][0][*strValue2].insert(&strValue1);

        writeErr("poset_add: poset " + to_string(id) + ", relation " + relation(value1, value2) + " added"); 
        return true;
    }

    bool poset_del(unsigned long id, char const* value1, char const* value2) {
        printAFunction(__func__, 3, id, value1, value2);
        if(checkForErrors(__func__, id, value1, value2, NULLVALUE1 | NULLVALUE2 | POSET_EXISTS 
           | VALUE1_IN_POSET | VALUE2_IN_POSET)) {
            return false;
        }
        
        const unique_ptr<string>& strValue1 = getRef(id, value1);
        const unique_ptr<string>& strValue2 = getRef(id, value2);

        if(strValue1 == strValue2 || relations()[id][1][*strValue1].count(&strValue2) == 0) {
            writeErr("poset_del: poset " + to_string(id) + ", relation " + relation(value1, value2) + " cannot be deleted"); 
            return false;
        }

        relations()[id][1][*strValue1].erase(&strValue2);
        relations()[id][0][*strValue2].erase(&strValue1);

        set<const unique_ptr<string>* > visited = set<const unique_ptr<string>* >();
        if(DFS(id, getRef(id, value1), getRef(id, value2), visited)) {
            writeErr("poset_del: poset " + to_string(id) + ", relation " + relation(value1, value2) + " cannot be deleted"); 
            relations()[id][1][*strValue1].insert(&strValue2);
            relations()[id][0][*strValue2].insert(&strValue1);
            return false;
        }

        for(auto lesser : relations()[id][0][*strValue1]) {
            if(*lesser == strValue1)
                continue;
            relations()[id][1][**lesser].insert(&strValue2);
            relations()[id][0][*strValue2].insert(lesser);
        }

        for(auto greater : relations()[id][1][*strValue2]) {
            if(*greater == strValue2)
                continue;
            relations()[id][1][*strValue1].insert(greater);
            relations()[id][0][**greater].insert(&strValue1);
        }

        writeErr("poset_del: poset " + to_string(id) + ", relation " + relation(value1, value2) + " deleted"); 
        return true;
    }

    bool poset_test(unsigned long id, char const* value1, char const* value2) {
        printAFunction(__func__, 3, id, value1, value2);
        if(checkForErrors(__func__, id, value1, value2, NULLVALUE1 | NULLVALUE2 | POSET_EXISTS
           | VALUE1_IN_POSET | VALUE2_IN_POSET)) {
            return false;
        }

        set<const unique_ptr<string>* > visited = set<const unique_ptr<string>* >();

        bool result = DFS(id, getRef(id, value1), getRef(id, value2), visited);

        writeErr("poset_test: poset " + to_string(id) + ", relation " + relation(value1, value2) + " " + ((result) ? "exists" : "does not exist")); 
        return result;
    }

    void poset_clear(unsigned long id) {
        printAFunction(__func__, 1, id);
        if(checkForErrors(__func__, id, NULL, NULL, POSET_EXISTS))
            return;

        relations()[id][0].clear();
        relations()[id][1].clear();
        values()[id].clear();

        writeErr("poset_clear: poset " + to_string(id) + " cleared"); 
    }
}
