/* -*- C++ -*-
   Copyright © 2014, Ryan Pavlik
   This file is part of xleapmouse.

   xleapmouse is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   xleapmouse is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with xleapmouse.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRIVCONFIG_H
#define TRIVCONFIG_H

template <typename T> void tc_write(std::ostream &os, T *val) {
    os << *val;
}

template <typename T> void tc_read(std::istream &is, T *val) {
    is >> *val;
}

class TrivialConfig {
public:
    enum Type {
        BOOL, INT, FLOAT,
        INTVEC, FLOATVEC
    };

    struct ConfigValBase {
        virtual void write(std::ostream &os) = 0;
        virtual void read(std::istream &is) = 0;
    };


    /* This could in theory be less stupid.  So could C++. */
    template <typename T> struct ConfigVal : public ConfigValBase {
        T *val;

        ConfigVal(T *v) { val = v; };

        virtual void write(std::ostream &os) { tc_write(os, val); }
        virtual void read(std::istream &is) { tc_read(is, val); }
    };

    typedef std::map<std::string, ConfigValBase*> ConfigMap;

private:
    ConfigMap _vals;

    void readLine(std::string &s) {
        std::stringstream ss(s);
        std::string name;

        ss >> name;
        readVal(name, ss);
    }

    void writeLine(std::string name, ConfigValBase *v, std::ostream &os) {
        os << name << " ";
        v->write(os);
        os << std::endl;
    }

public:
    template <typename T> void add(std::string name, T *v) {
        _vals[name] = new ConfigVal<T>(v);
    }

    void write(const char *filename) {
        std::ofstream os(filename);

        for(ConfigMap::iterator i = _vals.begin(); i != _vals.end(); ++i)
            writeLine(i->first, i->second, os);
    }

    void read(const char *filename) {
        std::ifstream is(filename);

        while(is.good()) {
            std::string s = "";
            std::getline(is, s);
            readLine(s);
        }
    }

    void readVal(std::string name, std::istream &is) {
        if(_vals.find(name) != _vals.end())
            _vals[name]->read(is);
    }
};

#endif // TRIVCONFIG_H
