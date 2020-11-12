#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <cassert>

// could be elements of an array or an object
std::string get_elements_string(const std::string& str, const std::size_t start, const char brkt)
{
    if (str.empty())
    {
        return {};
    }

    const std::vector<std::pair<char, char>> brackets = {{'[', ']'}, {'{', '}'}};
    auto bit = std::find_if(brackets.begin(), brackets.end(), [=](auto p) {
        return p.first == brkt;
    });
    if (bit == brackets.end())
    {
        return {};
    }
    assert(str[start] == brkt);

    std::stack<char> sc;
    std::size_t i;
    for (i = start; i < str.length(); ++i)
    {
        auto c = str[i];
        if (c == bit->second)
        {
            while (!sc.empty())
            {
                auto c_out = sc.top();
                sc.pop();
                if (c_out == bit->first)
                {
                    break;
                }
            }

            if (sc.empty())
            {
                break;
            }
        }
        else
        {
            sc.push(c);
        }
    }

    if (i == str.length())
    {
        return {};
    }

    return str.substr(start, i - start + 1);
}


bool is_value_string(const std::string& op_name, const std::string& key)
{
    return false;
}

std::string add_quote(const std::string& input_str)
{
    if (input_str.empty())
    {
        std::abort();
    }

    std::string str = input_str;

    auto pp_start = str.find_first_not_of(' ');
    str.insert(pp_start, 1, '"');
    auto pp_end =str.find_last_not_of(' ');
    if (pp_end == str.size() - 1)
    {
        str.append(1, '"');
    }
    else
    {
        str.insert(pp_end + 1, 1, '"');
    }

    return str;
}

std::string get_key(std::string key_str)
{
    if (key_str.empty())
    {
        std::abort();
    }

    auto pp_start = key_str.find_first_not_of(' ');
    key_str.erase(0, pp_start);
    auto pp_end = key_str.find_last_not_of(' ');
    if (pp_end < key_str.size())
    {
        key_str.erase(pp_end + 1);
    }

    return key_str;
}

//std::string conver_pair_to_json_string(const std::string& str)
//{
//    if (str.empty())
//    {
//        std::abort();
//    }
//
//    std::string result;
//
//    // processing key, add quote before and after the key
//    auto pos = str.find_first_not_of(':', 0);
//    if (pos == std::string::npos)
//    {
//        std::abort();
//    }
//
//    auto key_str = str.substr(0, pos);
//    result(add_quote(key_str));
//    result.append(": ");
//
//    auto key = get_key(key_str);
//
//    std::string val_str = str.substr(pos + 1);
//    // if the value should be a string
//    if (is_value_string(op_name, key))
//    {
//        val_str = add_quote(val_str);
//    }
//    result.append(val_str);
//
//    return result;
//}

std::string jsonize_object_string(const std::string& op_name, const std::string& str)
{
    if (str.empty())
    {
        return {};
    }

    assert(str.front() == '{');
    assert(str.back() == '}');

    std::string result = "{";
    std::size_t pos = 1;
    while (pos < str.length())
    {
        // processing key, add quote before and after the key
        auto c_pos = str.find(':', pos);
        if (c_pos == std::string::npos)
        {
            std::abort();
        }

        auto key_str = str.substr(pos, c_pos - pos);
        result.append(add_quote(key_str));
        result.append(": ");

        auto key = get_key(key_str);

        // if the value must be a string
        pos = c_pos + 1;
        if (is_value_string(op_name, key))
        {
            // get the next ',' charact and the substring
            // as the value
            auto v_end = str.find_first_of(',', pos);
            std::string val_str;
            if (v_end == std::string::npos)
            {
                val_str = str.substr(pos);
            }
            else
            {
                val_str = str.substr(pos, v_end - pos + 1);
            }
            result.append(add_quote(val_str));

            // reach string end, return
            if (v_end == std::string::npos)
            {
                result.append(1, '}');
                return result;
            }
            pos = v_end + 1;
        }
        // value can be another object or array or single element (not a string)
        // object need recursive processing, but array and single element donot need
        else
        {
            auto sp = str.find_first_not_of(' ', pos);
            if (sp == std::string::npos)
            {
                // no value, must be wrong
                std::abort();
            }

            // value is another object, need recursive call
            if (str[sp] == '{')
            {
                auto obj_str = get_elements_string(str, sp, '{');
                auto json_obj_str = jsonize_object_string(op_name, obj_str);
                if (!json_obj_str.empty())
                {
                    result.append(json_obj_str);

                }
                pos = sp + obj_str.length();
            }
            // value is an array
            else if (str[sp] == '[')
            {
                auto array_str = get_elements_string(str, sp, '[');
                // no array element are string, no additional processing
                result.append(array_str);
                pos = sp + array_str.length();
            }
            else
            {
                auto cp = str.find_first_of(',', pos);
                if (cp == std::string::npos)
                {
                    result.append(str.substr(pos));
                }
                else
                {
                    result.append(str.substr(pos, cp - pos));
                }
            }

            pos = str.find_first_of(',', pos);
            if (pos == std::string::npos)
            {
                result.append(1, '}');
                return result;
            }
            result.append(1, ',');

            ++pos;
        }
    }

    return result;
}

//int main()
//{
//    std::string str = "{abc : 1, def : {dim: [1, 2, 3, 4]}}";
//    std::cout << "str = " << str << std::endl;
//
//    auto json_str = jsonize_object_string("add", str);
//    std::cout << "json_str = " << json_str << std::endl;
//
//    return 0;
//}
