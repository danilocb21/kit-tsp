#include <vector>

#include "data.h"
#include "BnB.hpp"

BnB::BnB(Data &instance, double **cost) : m_instance(instance), costs(cost) {
    N = m_instance.getDimension();
}