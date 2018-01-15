#ifndef MODEL
#define MODEL

#include <vector>
#include <QAbstractTableModel>

#include "work.h"

struct Row {
    int input;
    bool stage1;
    bool stage2;
    bool done;
};

using Table = std::vector<Row>;

class Model:
        public QAbstractTableModel {
    Q_OBJECT
public:
    QVariant headerData(int section, Qt::Orientation orientation, int) const override {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return QString("input");
                case 1: return QString("stage1");
                case 2: return QString("stage2");
                case 3: return QString("done everything");
            }
        } else {
            return QString::number(section + 1);
        }
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return table.size();
    }

    int columnCount(const QModelIndex &) const override {
        return 4;
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (index.isValid() && role == Qt::DisplayRole) {
            const auto & row = table[index.row()];
            switch (index.column()) {
                case 0: return row.input;
                case 1: return row.stage1 ? QString("done"): QString("");
                case 2: return row.stage2 ? QString("done"): QString("");
                case 3: return row.done   ? QString("done"): QString("");
            }
        }
        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        auto currentFlags = QAbstractTableModel::flags(index);
        if (index.column() == 0) {
            currentFlags |= Qt::ItemIsEditable;
        }
        return currentFlags;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override {
        if (index.isValid() && role == Qt::EditRole) {
            auto & row = table[index.row()];
            switch (index.column()) {
                case 0: row.input = value.toInt();
                    break;
                case 1: row.stage1 = value.toBool();
                    break;
                case 2: row.stage2 = value.toBool();
                    break;
                case 3: row.done   = value.toBool();
                    break;
            }

            emit dataChanged(index, index);
            return true;
        }

        return false;
    }

    bool insertRows(int row, int count, const QModelIndex &parent) override {
        beginInsertRows(QModelIndex(), row, row + count - 1);

        table.insert(table.begin() + row,
                     count,
                     Row());

        endInsertRows();
        return true;
    }

    Work getWork() const {
        Work result;
        result.reserve(table.size());
        for (const auto & row: table) {
            result.emplace_back(row.input);
        }
        return result;
    }

private:
    Table table;
};

#endif // MODEL

