#ifndef DIALOGTOPOLOGY_H
#define DIALOGTOPOLOGY_H

#include <QDialog>

namespace Ui {
class DialogTopology;
}

class DialogTopology : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTopology(QWidget *parent = 0);
    ~DialogTopology();

public slots:
    void updateTopology(QImage topology);

private:
    Ui::DialogTopology *ui;
};

#endif // DIALOGTOPOLOGY_H
