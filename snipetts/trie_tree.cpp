#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <string>
#include <gtest/gtest.h>

using Num = long long int;

class TestAll : public ::testing::Test {};

struct TrieTreeNode {
    char chr {'*'};
    std::string value;
    size_t multiplicity {0};
    std::map<char, std::unique_ptr<TrieTreeNode>> children;

    TrieTreeNode() = default;

    void add(const std::string& s, size_t pos) {
        if (s.size() == pos) {
            value = s;
            ++multiplicity;
            return;
        }

        const auto& c = s.at(pos);
        if (!children.contains(c)) {
            children[c] = std::make_unique<TrieTreeNode>();
            children[c]->chr = c;
        }
        children[c]->add(s, pos + 1);
    }

    void add(const std::string& s) {
        add(s, 0);
    }

    void print_dfs(std::ostream& os) const {
        os << "(" << chr << ":" << value << ":" << multiplicity << ")\n";

        std::vector<std::pair<char, const TrieTreeNode*>> ptrs;
        for(const auto& [c, p] : children) {
            ptrs.push_back(std::make_pair(c, p.get()));
        }
        std::ranges::sort(ptrs);

        for(const auto& child : ptrs) {
            child.second->print_dfs(os);
        }
    }
};

TEST_F(TestAll, Isolate) {
    TrieTreeNode top;
    {
        std::string expected {"(*::0)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("d");
    {
        std::string expected {"(*::0)\n(d:d:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("ef");
    {
        std::string expected {"(*::0)\n(d:d:1)\n(e::0)\n(f:ef:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("aba");
    {
        std::string expected {"(*::0)\n(a::0)\n(b::0)\n(a:aba:1)\n(d:d:1)\n(e::0)\n(f:ef:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

TEST_F(TestAll, Overlap) {
    TrieTreeNode top;
    top.add("ef");
    top.add("efg");
    {
        std::string expected {"(*::0)\n(e::0)\n(f:ef:1)\n(g:efg:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("e");
    {
        std::string expected {"(*::0)\n(e:e:1)\n(f:ef:1)\n(g:efg:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

TEST_F(TestAll, Branch) {
    TrieTreeNode top;
    top.add("efg");
    top.add("efh");
    {
        std::string expected {"(*::0)\n(e::0)\n(f::0)\n(g:efg:1)\n(h:efh:1)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

TEST_F(TestAll, Duplicate) {
    TrieTreeNode top;
    top.add("efg");
    top.add("e");
    top.add("efg");
    {
        std::string expected {"(*::0)\n(e:e:1)\n(f::0)\n(g:efg:2)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }

    top.add("e");
    {
        std::string expected {"(*::0)\n(e:e:2)\n(f::0)\n(g:efg:2)\n"};
        std::ostringstream oss;
        top.print_dfs(oss);
        ASSERT_EQ(expected, oss.str());
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
