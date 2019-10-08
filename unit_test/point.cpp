#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "point.hpp"

using namespace bglex;

BOOST_AUTO_TEST_CASE(test1){
	Point p;

    BOOST_CHECK_EQUAL(p.getX(), 0);
    BOOST_CHECK_EQUAL(p.getY(), 0);
}

BOOST_AUTO_TEST_CASE(test2){
	Point p;
    p.setX(2);
    p.setY(3);

    BOOST_CHECK_EQUAL(p.getX(), 2);
    BOOST_CHECK_EQUAL(p.getY(), 3);
}
