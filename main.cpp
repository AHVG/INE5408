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

int main() {
    vector<string> tags = {"cenarios", "cenario", "nome", "dimensoes", "altura", "largura", "robo", "x", "y", "matriz"};

    XMLParser parser(tags);


    for (int i = 1; i < 7; i++) {
        ifstream arquivo;
        string xml = "";
        string file_name = "./cenarios/cenarios" + to_string(i) + ".xml";

        arquivo.open(file_name);

        if (arquivo.is_open())
        {
            string linha;
            while (getline(arquivo, linha)) xml += linha + "\n";
            arquivo.close();
        } else {
            cout << "Não foi possível abrir o arquivo!" << endl;
            continue;
        }

        if (parser.isValid(xml)) {
            cout << "Tudo certo com o arquivo: " + file_name << endl;

            vector<string> tag_hierarchy = {"cenario", "robo", "x"};
            for (auto content : parser.get_tags_contents(xml, tag_hierarchy))
                cout << content << endl << endl;
        }
    }

    return 0;
}