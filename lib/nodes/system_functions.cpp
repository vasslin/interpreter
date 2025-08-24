#include "system_functions.h"

namespace systemFunctions {

template <typename T>
T convertAny(std::any value, const std::string& name) {
    if (value.type().name() != typeid(T).name()) {
        throw std::runtime_error("Invalid  argument of function " + name);
    }
    return std::any_cast<T>(value);
}

// zero argument functions
std::any read() {
    std::string str;
    if (!std::getline(std::cin, str)) Nil{};
    return str;
}

std::any stacktrace() { return std::string{"stacktrace"}; }

std::unordered_map<std::string, std::function<std::any()>> zero_arg_functions =
    std::unordered_map<std::string, std::function<std::any()>>{{"read", read}, {"stacktrace", stacktrace}};

// one argument functions

// number functions
std::any abs(std::shared_ptr<Node> node) { return std::abs(convertAny<double>(node->visit(), std::string{"abs"})); }
std::any ceil(std::shared_ptr<Node> node) { return std::ceil(convertAny<double>(node->visit(), std::string{"ceil"})); }
std::any floor(std::shared_ptr<Node> node) {
    return std::floor(convertAny<double>(node->visit(), std::string{"floor"}));
}
std::any round(std::shared_ptr<Node> node) {
    return std::round(convertAny<double>(node->visit(), std::string{"round"}));
}
std::any sqrt(std::shared_ptr<Node> node) { return std::sqrt(convertAny<double>(node->visit(), std::string{"sqrt"})); }
std::any rnd(std::shared_ptr<Node> node) {
    auto num = convertAny<double>(node->visit(), std::string{"rnd"});
    if (num < 1) {
        return 0;
    }
    return (std::rand()) % (static_cast<size_t>(std::floor(num)));
}
std::any parse_num(std::shared_ptr<Node> node) {
    auto str = convertAny<std::string>(node->visit(), std::string{"parse_num"});
    if (str.size() < 1) {
        return Nil{};
    }
    return std::strtod(&str[0], nullptr);
}
std::any to_string(std::shared_ptr<Node> node) {
    auto num = convertAny<double>(node->visit(), std::string{"to_string"});
    return std::to_string(num);
}

// string functions

std::any len(std::shared_ptr<Node> node) {
    auto val = node->visit();

    if (val.type().name() == typeid(std::string).name()) {
        return static_cast<double>((convertAny<std::string>(val, std::string{"len"})).size());
    }
    return static_cast<double>((convertAny<std::shared_ptr<ListNode::data_type>>(val, std::string{"len"}))->size());
}
std::any lower(std::shared_ptr<Node> node) {
    auto str = convertAny<std::string>(node->visit(), std::string{"abs"});
    std::transform(str.begin(), str.end(), str.begin(), [](char c) { return std::tolower(c); });
    return str;
}
std::any upper(std::shared_ptr<Node> node) {
    auto str = convertAny<std::string>(node->visit(), std::string{"abs"});
    std::transform(str.begin(), str.end(), str.begin(), [](char c) { return std::toupper(c); });
    return str;
}

// system functions

std::any print(std::shared_ptr<Node> node);

void printString(std::string str, std::ostream* stream = &std::cout) {
    if (!stream) {
        return;
    }
    (*stream) << str;
}

void printNum(double num, std::ostream* stream = &std::cout) {
    if (!stream) {
        return;
    }
    (*stream) << num;
}

void printList(std::shared_ptr<ListNode::data_type> data, std::ostream* stream = &std::cout) {
    if (!stream) {
        return;
    }
    if (data->empty()) {
        (*stream) << "[]";
        return;
    }
    (*stream) << "[";
    for (size_t i = 0; i < data->size() - 1; ++i) {
        print((*data)[i]);

        (*stream) << ", ";
    }

    print(data->back());

    (*stream) << "]";
}

std::any print(std::shared_ptr<Node> node) {
    auto data = node->visit();

    if (data.type().name() == typeid(double).name()) {
        printNum(convertAny<double>(data, "print(x)"));
    } else if (data.type().name() == typeid(std::string).name()) {
        printString(convertAny<std::string>(data, "print(x)"));
    } else if (data.type().name() == typeid(std::shared_ptr<ListNode::data_type>).name()) {
        printList(convertAny<std::shared_ptr<ListNode::data_type>>(data, "print(x)"));
    } else if ((data).type().name() == typeid(Nil{}).name()) {
        std::cout << "nil";
    } else {
        std::cout << (data).type().name();
    }
    return Nil{};
}

std::any println(std::shared_ptr<Node> node) {
    print(node);
    std::cout << "\n";
    return Nil{};
}

// list funtions
std::any pop(std::shared_ptr<Node> node) {
    auto data = convertAny<std::shared_ptr<ListNode::data_type>>(node->visit(), std::string{"pop(list)"});
    if (data->empty()) {
        throw std::runtime_error("Pop operation of empty list is invalid.");
    }

    auto back_el = data->back();
    data->pop_back();
    return back_el->visit();
}

template <typename T>
bool the_same(ListNode::data_type& data) {
    for (auto node : data) {
        if ((node->visit().type().name()) != (typeid(T).name())) {
            return false;
        }
    }
    return true;
}

std::any sort(std::shared_ptr<Node> node) {
    auto data = node->visit();
    if (data.type().name() != typeid(std::shared_ptr<ListNode::data_type>).name()) {
        throw std::runtime_error("Sort function expected list argument.");
    }
    auto list_data = std::any_cast<std::shared_ptr<ListNode::data_type>>(data);
    if (the_same<double>(*list_data)) {
        std::sort(list_data->begin(), list_data->end(),
                  [](std::shared_ptr<FactorNode> n1, std::shared_ptr<FactorNode> n2) {
                      return (std::any_cast<double>(n1->visit()) < std::any_cast<double>(n2->visit()));
                  });
    } else if (the_same<std::string>(*list_data)) {
        std::sort(list_data->begin(), list_data->end(),
                  [](std::shared_ptr<FactorNode> n1, std::shared_ptr<FactorNode> n2) {
                      auto str1 = std::any_cast<std::string>(n1->visit());
                      auto str2 = std::any_cast<std::string>(n2->visit());
                      std::less<std::string> comp;
                      return comp(str1, str2);
                  });
    } else {
        std::sort(list_data->begin(), list_data->end(),
                  [](std::shared_ptr<FactorNode> n1, std::shared_ptr<FactorNode> n2) { return (n1.get() < n2.get()); });
    }
    return Nil{};
}

std::any range(std::shared_ptr<Node> n_node) {
    auto n = convertAny<double>(n_node->visit(), std::string{"range(x, y)"});

    ListNode::data_type data{};

    for (auto i = 0; i < n; ++i) {
        data.push_back(std::make_shared<NumNode>(i));
    }
    return std::make_shared<ListNode::data_type>(data);
}

std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>)>> one_arg_functions =
    std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>)>>{{"abs", abs},
                                                                                    {"ceil", ceil},
                                                                                    {"floor", floor},
                                                                                    {"round", round},
                                                                                    {"sqrt", sqrt},
                                                                                    {"rnd", rnd},
                                                                                    {"parse_num", parse_num},
                                                                                    {"to_string", to_string},
                                                                                    {"print", print},
                                                                                    {"println", println},
                                                                                    {"pop", pop},
                                                                                    {"len", len},
                                                                                    {"sort", sort},
                                                                                    {"range", range}

    };

// two args functions

std::any split(std::shared_ptr<Node> s_node, std::shared_ptr<Node> delim_node) {
    auto str = convertAny<std::string>(s_node->visit(), std::string{"split(s, delim)"});
    auto delim = convertAny<std::string>(delim_node->visit(), std::string{"split(s, delim)"});

    ListNode::data_type data{};

    if (delim.size() > str.size()) {
        return data;
    }
    std::string tmp = "";
    for (size_t i = 0; i < str.size();) {
        if (i <= str.size() - delim.size() && str.substr(i, delim.size()) == delim) {
            if (!tmp.empty()) {
                data.push_back(std::make_shared<StringNode>(tmp));
                tmp.clear();
            }
            i += delim.size();
        } else {
            tmp.push_back(str[i]);
            ++i;
        }
    }
    if (!tmp.empty()) {
        data.push_back(std::make_shared<StringNode>(tmp));
    }
    return std::make_shared<ListNode::data_type>(data);
}

std::any join(std::shared_ptr<Node> list_node, std::shared_ptr<Node> delim_node) {
    auto data =
        *(convertAny<std::shared_ptr<ListNode::data_type>>(list_node->visit(), std::string{"join(list, delim)"}));
    auto delim = convertAny<std::string>(delim_node->visit(), std::string{"join(list, delim)"});

    std::string ans = "";

    if (data.empty()) {
        return ans;
    }

    for (size_t i = 0; i < data.size() - 1; ++i) {
        ans += convertAny<std::string>(data[i]->visit(), std::string{"join(list, delim)"});
        ans += delim;
    }
    ans += convertAny<std::string>(data.back()->visit(), std::string{"join(list, delim)"});
    return ans;
}

// list functions

std::any push(std::shared_ptr<Node> list_node, std::shared_ptr<Node> x_node) {
    auto data = convertAny<std::shared_ptr<ListNode::data_type>>(list_node->visit(), std::string{"push(list, x)"});
    auto ptr = std::dynamic_pointer_cast<FactorNode>(x_node);
    if (!ptr) {
        throw std::runtime_error("Pop function expected list argument.");
    }
    data->push_back(ptr);
    return Nil{};
}

std::any remove(std::shared_ptr<Node> list_node, std::shared_ptr<Node> index_node) {
    auto data =
        convertAny<std::shared_ptr<ListNode::data_type>>(list_node->visit(), std::string{"remove(list, index)"});
    auto index = static_cast<size_t>(convertAny<double>(index_node->visit(), std::string{"remove(list, index)"}));

    if (index >= data->size()) {
        return Nil{};
    }

    data->erase(data->begin() + index);
    return Nil{};
}

std::any range2(std::shared_ptr<Node> x_node, std::shared_ptr<Node> y_node) {
    auto x = convertAny<double>(x_node->visit(), std::string{"range(x, y)"});
    auto y = convertAny<double>(y_node->visit(), std::string{"range(x, y)"});

    ListNode::data_type data{};

    for (auto i = x; i < y; ++i) {
        data.push_back(std::make_shared<NumNode>(i));
    }
    return std::make_shared<ListNode::data_type>(data);
}

std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>)>>
    two_args_functions =
        std::unordered_map<std::string, std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>)>>{
            {"join", join}, {"split", split}, {"push", push}, {"remove", remove}, {"range", range2}};

// three args dunction
std::any replace(std::shared_ptr<Node> s_node, std::shared_ptr<Node> old_node, std::shared_ptr<Node> new_node) {
    auto str = convertAny<std::string>(s_node->visit(), std::string{"replace(s, old, new)"});
    auto old_s = convertAny<std::string>(old_node->visit(), std::string{"replace(s, old, new)"});
    auto new_s = convertAny<std::string>(new_node->visit(), std::string{"replace(s, old, new)"});

    auto ind = str.find(old_s);
    if (ind == std::string::npos) {
        return Nil{};
    }
    str.replace(ind, old_s.size(), new_s);
    return str;
}

std::any range3(std::shared_ptr<Node> x_node, std::shared_ptr<Node> y_node, std::shared_ptr<Node> step_node) {
    auto x = convertAny<double>(x_node->visit(), std::string{"range(x, y, step)"});
    auto y = convertAny<double>(y_node->visit(), std::string{"range(x, y, step)"});
    auto step = convertAny<double>(step_node->visit(), std::string{"range(x, y, step)"});

    ListNode::data_type data{};

    for (auto i = x; i < y; i += step) {
        data.push_back(std::make_shared<NumNode>(i));
    }
    return std::make_shared<ListNode::data_type>(data);
}

std::any insert(std::shared_ptr<Node> list_node, std::shared_ptr<Node> index_node, std::shared_ptr<Node> x_node) {
    auto data =
        convertAny<std::shared_ptr<ListNode::data_type>>(list_node->visit(), std::string{"insert(list, index, x)"});
    auto index = static_cast<size_t>(convertAny<double>(index_node->visit(), std::string{"insert(list, index, x)"}));

    auto ptr = std::dynamic_pointer_cast<FactorNode>(x_node);
    if (!ptr) {
        throw std::runtime_error("Pop function expected list argument.");
    }

    if (index >= data->size()) {
        data->push_back(ptr);
    } else {
        data->insert(data->begin() + index, ptr);
    }
    return Nil{};
}

std::unordered_map<std::string,
                   std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>)>>
    three_args_functions = std::unordered_map<
        std::string, std::function<std::any(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>)>>{
        {"replace", replace}, {"range", range3}, {"insert", insert}

    };

}  // namespace systemFunctions