#pragma once
#include "polynom.h"
#include <string>

using namespace std;

extern const char* const TABLE_NAMES[];
extern const int TABLE_COUNT;

// ����������� ������� �������
class Table {
public:
    Table(Polynom* data = nullptr, size_t size = 0);
    virtual ~Table() = default;

    virtual Polynom* find(const string& name) = 0;
    virtual void add(const string& name, const Polynom* pol) = 0;
    virtual void remove(const string& name) = 0;
};

// ������� �� ������ ��������� �������
class LinearArrayTable : public Table {
public:
    LinearArrayTable();
    virtual ~LinearArrayTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// ������� �� ������ �������� ������
class LinearListTable : public Table {
public:
    LinearListTable();
    virtual ~LinearListTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// ������������� ������� �� ������ �������
class OrderedArrayTable : public Table {
public:
    OrderedArrayTable();
    virtual ~OrderedArrayTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// ������� �� ������ ��������� ������ ������
class TreeTable : public Table {
public:
    TreeTable();
    virtual ~TreeTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// ���-������� � �������� ����������
class OpenHashTable : public Table {
public:
    OpenHashTable();
    virtual ~OpenHashTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// ���-������� � ���������
class ListHashTable : public Table {
public:
    ListHashTable();
    virtual ~ListHashTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};
