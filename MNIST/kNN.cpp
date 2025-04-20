#include "kNN.hpp"
//class Image//
template<typename T>
Image<T>::Image() {
    this->head = this->tail = nullptr;
    this->size = 0;
}
template <typename T>
Image<T>::~Image() {
    this->clear();
}
template<typename T>
void Image<T>::push_back(T value) {
    if (size == 0) {
        head = tail = new Node(value);
        size = 1;
    }
    else {
        tail->next = new Node(value);
        tail = tail->next;
        size++;
    }

}
template <typename T>
void Image<T>::push_front(T value) {
    Node* NewNode = new Node(value);
    if (size == 0) {
        head = tail = NewNode;
        size = 1;
    }
    else {
        NewNode->next = head;
        head = NewNode;
        size++;
    }
}
template <typename T>
void Image<T>::insert(int index, T value) {
    if (index < 0 || index > size) return;
    else {
        if (index == 0) {
            push_front(value);
        }
        else if (index == size) {
            push_back(value);
        }
        else {
            Node* NewNode = new Node(value);
            Node* tmp = head;
            for (int i = 0; i < index - 1; i++) {
                tmp = tmp->next;
            }
            NewNode->next = tmp->next;
            tmp->next = NewNode;
            size++;
        }
    }
}
template <typename T>
void Image<T>::remove(int index) {
    if (index < 0 || index >= size || length() == 0) return;
    if (index == 0) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
    }
    else {
        Node* tmp1 = head;
        for (int i = 0; i < index-1; i++) {
            tmp1 = tmp1->next;
        }
        Node* tmp2 = tmp1->next;
        tmp1->next = tmp2->next;
        if (tmp1->next == nullptr) {
            tail = tmp1;
        }
        delete tmp2;
    }
    size--;
}
template <typename T>
T& Image<T>::get(int index) const {
    if (index < 0 || index >= this->size)  throw std::out_of_range("get(): Out of range");
    Node* temp = head;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    }
    return temp->pointer;
}
template <typename T>
int Image<T>::length() const {
    return size;
}
template <typename T>
void Image<T>::clear() {
    Node* tmp = head;
    while (head != nullptr) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
    size = 0;
    tail = head = nullptr;
}
template <typename T>
void Image<T>::print() const {
    if (size == 0) cout <<"nullptr" << endl;
    Node* temp = head;
    for (int i = 0; i < this->size; i++)
    {
        if (i == this->size - 1) {
            cout << temp->pointer;
        }
        else{
        cout << temp->pointer << " ";
        }
        temp = temp->next;
    }
}
template <typename T>
void Image<T>::reverse() {
    Node* cur = head;
    Node* pre = NULL;
    Node* next = NULL;

    while (cur != NULL) {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    tail = head;
    head = pre;
}
template <typename T>
void Image<T>::printStartToEnd(int start, int end) const
{
    Node* temp = head;
    for (int i = 0; i < start; i++)
        temp = temp->next;
    for (int i = start; i < end && i < this->size; i++)
    {
        if (i == end - 1 || i == this->size - 1)
            cout << temp->pointer << endl;
        else
            cout << temp->pointer << " ";
        temp = temp->next;
    }
}
template <typename T>
List<T>* Image<T>::subList(int start, int end) {
    if (this->size <= start) return nullptr;
    if (end > this->size) {
        end = size;
    }
    List<T>* result = new Image<T>();
    Node* tmp = head;
    for (int i = 0; i < start; i++) {
        tmp = tmp->next;
    }
    for (int i = start; i < end; i++) {
        result->push_back(tmp->pointer);
        tmp = tmp->next;
        if (tmp == nullptr)
        {
            break;
        }
    }
    return result;
}
template <typename T>
void Image<T>::getArray(T*& Array)const  {
    Node* tmp = head;
    int i = 0;
    while (tmp != nullptr) {
        Array[i] = tmp->pointer;
        tmp = tmp->next;     
        i++;
    }
}
//class Dataset//
Dataset::Dataset()
{
    this->nameCol = new Image<string>();
    this->data = new Image<List<int>*>();
}
Dataset::~Dataset()
{
    for (int i = 0; i < this->data->length(); i++) {
        data->get(i)->clear();

    }
    for (int i = 0; i < data->length(); i++) {
        data->get(i)->clear();
    }
    delete data;
    delete nameCol;
}
Dataset::Dataset(const Dataset& other)
{
    this->nameCol = new Image<string>();
    this->data = new Image<List<int>*>();
    for (int i = 0; i < other.data->length(); i++) {
        List<int>* newData = new Image<int>();
        for (int j = 0; j < other.data->get(i)->length(); j++) {
            newData->push_back(other.data->get(i)->get(j));
        }
        this->data->push_back(newData);
    }
    for (int i = 0; i < other.nameCol->length(); i++) {
        this->nameCol->push_back(other.nameCol->get(i));
    }
}
Dataset& Dataset::operator=(const Dataset& other)
{
    if (this == &other) {
        return *this;
    }
    this->~Dataset();
    this->nameCol = new Image<string>();
    this->data = new Image<List<int>*>();
    if (other.data == nullptr) {
        for (int i = 0; i < other.nameCol->length(); i++) {
            this->nameCol->push_back(other.nameCol->get(i));
        }
        return *this;
    }
    for (int i = 0; i < other.nameCol->length(); i++) {
        this->nameCol->push_back(other.nameCol->get(i));
    }
    for (int i = 0; i < other.data->length(); ++i) {
        List<int>* tmp = new Image<int>();
        for (int j = 0; j < other.data->get(i)->length(); ++j) {
            tmp->push_back(other.data->get(i)->get(j));
        }
        this->data->push_back(tmp);
    }
    return *this;
}
bool Dataset::loadFromCSV(const char* fileName)
{
    ifstream file(fileName);
    if (file.is_open())
    {
        string str;
        int number;
        file >> str;
        int tmp = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == ',') {
                nameCol->push_back(str.substr(tmp, i - tmp));
                tmp = i + 1;
            }
            else if (i == str.length() - 1) {
                nameCol->push_back(str.substr(tmp, i + 1 - tmp));
            }
        }
        while (file >> str)
        {
            for (int i = 0; i < str.length(); i++) {
                if (str[i] == ',') str[i] = ' ';
            }
            stringstream ss(str);
            List<int>* temp = new Image<int>();
            while (ss >> number)
            {
                temp->push_back(number);
            }
            data->push_back(temp);
        }
        return true;
    }
    return false;
}
void Dataset::getShape(int& nRows, int& nCols) const
{
    nRows = data->length();
    if (nRows != 0) {
        nCols = data->get(0)->length();
    }
    else {
        throw std::out_of_range("get(): Out of range");
        nCols = 0;
    }
}
void Dataset::columns() const
{
    for (int i = 0; i < this->nameCol->length(); i++) {
        if (i == this->nameCol->length() - 1) {
            cout << this->nameCol->get(i);
        }
        else {
            cout << this->nameCol->get(i) << " ";
        }
    }

}
void Dataset::printHead(int nRows, int nCols) const
{
    if (nRows <= 0 || nCols <= 0 || this->data->length() == 0) {
        for (int i = 0; i < nameCol->length(); i++) {
            if (i == nameCol->length() - 1) {
                cout << nameCol->get(i);
            }
            else {
                cout << nameCol->get(i) << " ";
            }
        }
        return;
    }
    if (nRows > this->data->length()) {
        nRows = this->data->length();
    }
    if (nCols > this->data->get(0)->length()) {
        nCols = this->data->get(0)->length();
    }
    nameCol->printStartToEnd(0, nCols);
    for (int i = 0; i < nRows; i++) {
        if (i == nRows - 1) {
            for (int j = 0; j < nCols; j++) {
                if (j == nCols - 1) {
                    cout << this->data->get(i)->get(j);
                }
                else {
                    cout << this->data->get(i)->get(j) << " ";

                }
            }
        }
        else {
            this->data->get(i)->printStartToEnd(0, nCols);
        }
    }
}
void Dataset::printTail(int nRows, int nCols) const
{
    if (nRows <= 0 || nCols <= 0 || this->data->length() == 0) return;
    if (nRows > this->data->length()) {
        nRows = this->data->length();
    }
    if (nCols > this->data->get(0)->length()) {
        nCols = this->data->get(0)->length();
    }
    if (nCols <= this->data->get(0)->length()) {
        nameCol->printStartToEnd(this->nameCol->length() - nCols, this->nameCol->length());
        for (int i = this->data->length() - nRows; i < this->data->length() -1; i++) {
            this->data->get(i)->printStartToEnd(this->data->get(0)->length() - nCols, this->data->get(0)->length());
            if (i == this->data->length() - 2) {
                for (int j = this->data->get(0)->length() - nCols; j < this->data->get(0)->length(); j++) {
                    if (j == this->data->get(0)->length() - 1) {
                        cout << this->data->get(i)->get(j);
                    }
                    else {
                        cout << this->data->get(i)->get(j) << " ";
                    }
                }
            }
        }
    }
    else {
        nameCol->printStartToEnd(0, this->nameCol->length());
        for (int i = this->data->length() - nRows + 1; i < this->data->length(); i++) {
            this->data->get(i)->printStartToEnd(0, nCols);
            if (i == this->data->length() - 1) {
                for (int j = 0; j < nCols; j++) {
                    if (j == nCols - 1) {
                        cout << this->data->get(i)->get(j);
                    }
                    else {
                        cout << this->data->get(i)->get(j) << " ";
                    }
                }
            }
        }
    }
}
bool Dataset::drop(int axis, int index, string columns)
{
    if (axis == 0 && this->data->length() != 0) {
        if (index < this->data->length() && index >= 0) {
            for (int i = 0; i < this->data->get(index)->length(); i++) {
                this->data->remove(index);
                return true;
            }
        }
    }
    else if (axis == 1 && this->data->length() != 0) {
        int tmp = -1;
        for (int i = 0; i <= nameCol->length(); i++) {
            if (columns == nameCol->get(i)) {
                tmp = i;
                nameCol->remove(tmp);
                break;
            }
            else if (i == nameCol->length() - 1 && tmp == -1) {
                return false;
            }
        }
        for (int j = 0; j < this->data->length(); j++) {
            this->data->get(j)->remove(tmp);
        }

        return true;
    }
    return false;
}
Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const
{
    Dataset image = Dataset();
    if (this->data->length() == 0 || this->data->get(0)->length() == 0 || startRow >= this->data->length() || startCol >= this->data->get(0)->length()) {
        return image;
    }
    else {
        if (endRow == -1 || endRow > this->data->length()) {
            endRow = this->data->length();
        }
        if (endCol == -1) {
            endCol = this->data->get(0)->length();
        }

        for (int i = startRow; i <= endRow && i < this->data->length(); i++) {
            List<int>* newData = new Image<int>();
            for (int j = startCol; j <= endCol && j < this->data->get(0)->length(); j++) {
                newData->push_back(this->data->get(i)->get(j));
            }
            image.data->push_back(newData);
        }
        for (int i = startCol; i <= endCol && i < this->data->get(0)->length(); i++) {
            if (i < nameCol->length()) {
                image.nameCol->push_back(nameCol->get(i));
            }
        }
        return image;
    }
}
double Dataset::distanceEuclidean(const List<int>* x, const List<int>* y) const {
    int* ArrayX = new int[x->length()];
    int* ArrayY = new int[y->length()];
    x->getArray(ArrayX);
    y->getArray(ArrayY);
    double distance = 0.0;
    int size = 0;
    while (size != x->length()) {
        int X = 0, Y = 0;
        X = ArrayX[size];
        Y = ArrayY[size];
        distance = distance + (X - Y) * (X - Y);
        size++;
    }
    delete[] ArrayX;
    delete[] ArrayY;
    return sqrt(distance);
}
void Dataset::InsertSort(List<int>*& distance, List<int>*& label, int size) const {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (distance->get(j) > distance->get(j + 1)) {
                swap(distance->get(j), distance->get(j + 1));
                swap(label->get(j), label->get(j + 1));
            }
        }
    }
}
Dataset Dataset::predict(const Dataset& X_train, const Dataset& Y_train, const int k) const {
    Dataset result;
    List<int>* distance = new Image<int>();
    List<int>* label = new Image<int>();
    if (X_train.data->length() == 0 || Y_train.data->length() == 0 || k <= 0 || k > X_train.data->length()) {
        return result;
    }
    result.nameCol = new Image<string>();
    for (int i = 0; i < Y_train.nameCol->length(); i++) {
        result.nameCol->push_back(Y_train.nameCol->get(i));
    }
    for (int i = 0; i < this->data->length(); i++) {
        for (int j = 0; j < X_train.data->length(); j++) {
            distance->push_back(distanceEuclidean(this->data->get(i), X_train.data->get(j)));
            label->push_back(j);
        }
        InsertSort(distance, label, X_train.data->length());
        int maxLabel = -1;
        int max = -1;
        int counts[10] = { 0 };
        for (int j = 0; j < k; j++) {
            int labels = Y_train.data->get(label->get(j))->get(0);
            counts[labels]++;
            if (counts[labels] > max || (counts[labels] == max && labels < maxLabel)) {
                max = counts[labels];
                maxLabel = labels;
            }
        }
        List<int>* nRow = new Image<int>();
        nRow->push_back(maxLabel);
        result.data->push_back(nRow);
        distance->clear();
        label->clear();
    }

    return result;
}
double Dataset::score(const Dataset& y_predict) const
{
    if (this->data->length() <= 0 || y_predict.data->length() <= 0 || (this->data->length() != y_predict.data->length())) {
        return -1;
    }
    else {
        int same = 0;
        for (int i = 0; i < this->data->length(); i++) {
            if (this->data->get(i)->get(0) == y_predict.data->get(i)->get(0)) {
                same++;
            }
        }
        return (double)same / y_predict.data->length();
    }
}
List<List<int>*>* Dataset::getData() const {
    return data;
}
//class kNN//
void kNN::fit(const Dataset& X_train, const Dataset& y_train)
{
    this->X_train = X_train;
    this->Y_train = y_train;
}
Dataset kNN::predict(const Dataset& X_test)
{
    return X_test.predict(this->X_train, this->Y_train, this->k);
}
double kNN::score(const Dataset& y_test, const Dataset& y_pred)
{
    return y_test.score(y_pred);
}
//train_test_split//
void train_test_split(Dataset& X, Dataset& Y, double test_size, Dataset& X_train, Dataset& X_test, Dataset& Y_train, Dataset& Y_test)
{
    if (X.getData()->length() != Y.getData()->length() || test_size >= 1 || test_size <= 0) {
        return;
    }
    int nRow = X.getData()->length();
    double rowSplit = nRow * (1 - test_size);
    X_train = X.extract(0, rowSplit - 1, 0, -1);
    Y_train = Y.extract(0, rowSplit - 1, 0, -1);
    X_test = X.extract(rowSplit, -1, 0, -1);
    Y_test = Y.extract(rowSplit, -1, 0, -1);
}