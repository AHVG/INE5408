#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;


class XMLParser {
 private:

    map<string, string> tags;

    string remove_chars(string text, string chars) {
        string new_text = text;
        for (auto c : chars)
            new_text.erase(remove(new_text.begin(), new_text.end(), c), new_text.end());

        return new_text;
    }

 public:
    explicit XMLParser(vector<string> tags_) {
        for (auto tag : tags_)
            tags["<" + tag + ">"] = "</" + tag + ">";
    }

    bool isValid(string xml) {
        
        string new_xml = remove_chars(xml, "\n");

        size_t xml_len = new_xml.length();
        stack<string> tags_stack;

        for (size_t i = 0; i < xml_len; i++) {
            for (auto tag : tags)
                if (tag.first.length() <= xml_len - i)
                    if (new_xml.substr(i, tag.first.length()) == tag.first)
                        tags_stack.push(tag.first);
            
            for (auto tag : tags)
                if (tag.second.length() <= xml_len - i)
                    if (new_xml.substr(i, tag.second.length()) == tag.second) {
                        if (tags_stack.empty())
                            return false;
                        else if (tags[tags_stack.top()] == tag.second)
                            tags_stack.pop();
                        else
                            return false;
                    }

        }

        return tags_stack.empty();
    }

    vector<string> get_tags_contents(string xml, string tag) {
        vector<string> tag_contents;
        tag = "<" + tag + ">";

        bool has = false;
        for (auto t : tags)
            if (t.first == tag)
                has = true;
        
        if (!has)
            return tag_contents;


        while(true) {

            size_t opening_tag_index = xml.find(tag);

            if (opening_tag_index == string::npos)
                break;

            opening_tag_index += tag.length();


            size_t closing_tag_index = xml.find(tags[tag]);

            size_t size = closing_tag_index - opening_tag_index;

            tag_contents.push_back(xml.substr(opening_tag_index, size));

            size = xml.length() - closing_tag_index + 1;

            xml = xml.substr(closing_tag_index + tags[tag].length(), size);

        }

        return tag_contents;
    }

    vector<string> get_tags_contents(string xml, vector<string> tag_hierarchy) {
        vector<string> contents;
        for (auto tag : tag_hierarchy) {
            string aux = "";
            contents = get_tags_contents(xml, tag);
            for (auto content : contents)
                aux += content;
            xml = aux;
        }
        return contents;
    }

};

class Point {

 public:

    Point() {}

    Point(const Point &other) {
        x = other.x;
        y = other.y;
    }

    Point(int x_, int y_) {
        x = x_;
        y = y_;
    }

    const Point &operator=(const Point &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    int x;
    int y;

};


class Scenario {

 public:

    Scenario(string name_, Point dimension_, Point robot_, string matrix_) {
        name = name_;
        dimension = dimension_;
        robot = robot_;

        matrix_.erase(remove(matrix_.begin(), matrix_.end(), '\n'), matrix_.end());
        
        int x = 0;
        int y = 0;
        for (int i = 0; i < dimension.y; i++)
            matrix.push_back(vector<int>());

        for (auto c : matrix_) {
            matrix.at(y).push_back(int(c) - int('0'));
            x++;
            if (x >= dimension.x) {
                x = 0;
                y++;
            }
        }
    }

    string name;
    vector<vector<int>> matrix;

    Point dimension;
    Point robot;

};

class ScenarioFactory {

 public:

    Scenario *create(string name, string width, string height, string robot_x, string robot_y, string matrix) {
        return new Scenario(name, Point(stoi(width), stoi(height)), Point(stoi(robot_x), stoi(robot_y)), matrix);
    }

    vector<Scenario *> create(vector<string> names, vector<string> widths, vector<string> heights, vector<string> robot_xs, vector<string> robot_ys, vector<string> matrices) {
        vector<Scenario *> scenarios = vector<Scenario *>();
        for (int i = 0; i < names.size(); i++)
            scenarios.push_back(create(names.at(i), widths.at(i), heights.at(i), robot_xs.at(i), robot_ys.at(i), matrices.at(i)));
        return scenarios;
    }
    
    void destroy(vector<Scenario *> scenarios) {
        for (auto &scenario : scenarios)
            delete scenario;
    }

};





class ScenarioAnalyzer {


 public:

    int analyze(Scenario *scenario) {
        stack<Point> way;
        vector<vector<int>> R;
        int sum = 0;

        for (int i = 0; i < scenario->matrix.size(); i++) {
            R.push_back(vector<int>());
            for (int j = 0; j < scenario->matrix.at(i).size(); j++)
                R.at(i).push_back(0);
        }

        Point aux = Point(scenario->robot.y, scenario->robot.x);
        way.push(aux);

        if (aux.x < scenario->dimension.x && aux.x >= 0 &&
            aux.y < scenario->dimension.y && aux.y >= 0)
            if (!scenario->matrix.at(aux.y).at(aux.x))
                return sum;

        while (!way.empty()) {
            Point p = way.top();
            way.pop();

            R.at(p.y).at(p.x) = 1;

            for (int i = -1; i < 2; i += 2)
                if (p.y + i < scenario->dimension.y && p.y + i > -1 &&
                    p.x < scenario->dimension.x && p.x > -1)
                    if (scenario->matrix.at(p.y + i).at(p.x) && !R.at(p.y + i).at(p.x))
                        way.push(Point(p.x, p.y + i));

            for (int i = -1; i < 2; i += 2)
                if (p.x + i < scenario->dimension.x && p.x + i > -1 &&
                    p.y < scenario->dimension.y && p.y > -1)
                    if (scenario->matrix.at(p.y).at(p.x + i) && !R.at(p.y).at(p.x + i))
                        way.push(Point(p.x + i, p.y));
        }

        for (auto line : R)
            for (auto e : line)
                sum += e;

        return sum;
            
    }


};


class Solver {

 private:

    XMLParser *parser;


 public:

    Solver() {
        vector<string> tags = {"cenarios", "cenario", "nome", "dimensoes", "altura", "largura", "robo", "x", "y", "matriz"};
        parser = new XMLParser(tags);
    }

    ~Solver() {
        delete parser;
    }

    void solve() {
        char file_name[100];

        std::cin >> file_name;

        ifstream file;
        string xml = "";
        file.open(file_name);
        string line;
        while (getline(file, line)) xml += line + "\n";
        file.close();

        if (!parser->isValid(xml)) {
            cout << "erro" << endl << endl;
            return;
        }
        
        vector<string> names = parser->get_tags_contents(xml, "nome");
        vector<string> xs = parser->get_tags_contents(xml, "largura");
        vector<string> ys = parser->get_tags_contents(xml, "altura");
        vector<string> robot_xs = parser->get_tags_contents(xml, "x");
        vector<string> robot_ys = parser->get_tags_contents(xml, "y");
        vector<string> matrices = parser->get_tags_contents(xml, "matriz");

        ScenarioFactory factory;
        ScenarioAnalyzer analyzer;
        vector<Scenario *> scenarios = factory.create(names, xs, ys, robot_xs, robot_ys, matrices);
        for (auto scenario : scenarios)
            cout << scenario->name << " " << analyzer.analyze(scenario) << endl;
        cout << endl;
        
        factory.destroy(scenarios);
    }

};

int main() {

    Solver *solver = new Solver();

    solver->solve();

    delete solver;

    return 0;
}