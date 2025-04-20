#include "kNN.hpp"

void tc1() {
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    List<int>* row = dataset.getData()->get(0);

    for (int i = 0; i < 1000; i++)
    {
        row->push_back(1);
    }

    cout << row->length();
}

int main() {
    tc1();
    return 0;
}