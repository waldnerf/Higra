//
// Created by user on 3/12/18.
//
#include <boost/test/unit_test.hpp>

#include "graph.hpp"
#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(boost_neighbouringgraph);

    using namespace hg;
    using namespace std;


    struct _data {

        hg::regular_grid_graph g;

        _data() {
            hg::embedding_grid embedding{3, 2}; // 3 rows, 2 columns
            std::vector<xt::xarray<long>> neighbours{{0,  -1},
                                                     {-1, 0},
                                                     {1,  0},
                                                     {0,  1}}; // 4 adjacency

            g = hg::regular_grid_graph(embedding, neighbours);
        }

    } data;

    BOOST_AUTO_TEST_CASE(sizeRegularGraph) {
        auto g = data.g;

        BOOST_CHECK(num_vertices(g) == 6);
    }


    BOOST_AUTO_TEST_CASE(vertexIteratorSimpleGraph) {

        auto g = data.g;

        vector<ulong> vref{0, 1, 2, 3, 4, 5};
        vector<ulong> vtest;

        for (auto v: hg::graphVertexIterator(g)) {
            vtest.push_back(v);
        }

        BOOST_CHECK(vectorEqual(vref, vtest));

    }
/*
    BOOST_AUTO_TEST_CASE(edgeIteratorSimpleGraph) {

        auto g = data.g;

        vector<pair<ulong, ulong>> eref{{0, 1},
                                        {1, 2},
                                        {0, 2}};
        vector<pair<ulong, ulong>> etest;
        for (auto e: hg::graphEdgeIterator(g))
            etest.push_back({source(e, g), target(e, g)});
        BOOST_CHECK(vectorEqual(eref, etest));
    }

    */

    BOOST_AUTO_TEST_CASE(outEdgeIteratorNeighbouringGraph) {

        auto g = data.g;

        vector<vector<pair<ulong, ulong>>> outListsRef{{{0, 1}, {0, 3}},
                                                       {{1, 0}, {1, 2}, {1, 4}},
                                                       {{2, 1}, {2, 5}},
                                                       {{3, 0}, {3, 4}},
                                                       {{4, 1}, {4, 3}, {4, 5}},
                                                       {{5, 2}, {5, 4}}
        };
        vector<vector<pair<ulong, ulong>>> outListsTest;

        for (size_t v = 0; v < 6; v++) {
            outListsTest.push_back({});
            for (auto e: hg::graphOutEdgeIterator(v, g))
                outListsTest[v].push_back({source(e, g), target(e, g)});
            BOOST_CHECK(vectorEqual(outListsRef[v], outListsTest[v]));
            BOOST_CHECK(out_degree(v, g) == outListsRef[v].size());

        }

    }

    BOOST_AUTO_TEST_CASE(inEdgeIteratorSimpleGraph) {

        auto g = data.g;

        vector<vector<pair<ulong, ulong>>> inListsRef{{{1, 0}, {3, 0}},
                                                      {{0, 1}, {2, 1}, {4, 1}},
                                                      {{1, 2}, {5, 2}},
                                                      {{0, 3}, {4, 3}},
                                                      {{1, 4}, {3, 4}, {5, 4}},
                                                      {{2, 5}, {4, 5}}
        };
        vector<vector<pair<ulong, ulong>>> inListsTest;

        for (auto v: hg::graphVertexIterator(g)) {
            inListsTest.push_back(vector<pair<ulong, ulong>>());
            for (auto e: hg::graphInEdgeIterator(v, g))
                inListsTest[v].push_back({source(e, g), target(e, g)});
            BOOST_CHECK(vectorEqual(inListsRef[v], inListsTest[v]));
            BOOST_CHECK(in_degree(v, g) == inListsRef[v].size());
            BOOST_CHECK(degree(v, g) == inListsRef[v].size());
        }
    }


    BOOST_AUTO_TEST_CASE(adjacentVertexIteratorSimpleGraph) {

        auto g = data.g;

        vector<vector<ulong>> adjListsRef{{1, 3},
                                          {0, 2, 4},
                                          {1, 5},
                                          {0, 4},
                                          {1, 3, 5},
                                          {2, 4}};
        vector<vector<ulong>> adjListsTest;

        for (auto v: hg::graphVertexIterator(g)) {
            adjListsTest.push_back(vector<ulong>());
            for (auto av: hg::graphAdjacentVertexIterator(v, g)) {
                adjListsTest[v].push_back(av);
            }
            BOOST_CHECK(vectorEqual(adjListsRef[v], adjListsTest[v]));
        }
    }

BOOST_AUTO_TEST_SUITE_END();