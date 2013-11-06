#ifndef THEMA_H
#define THEMA_H
#include <QObject>
#include <QDomElement>
#include <QVector>
#include "word.h"
#include "Thema_I.h"

class Thema_C : public QObject, public Thema_I
{

    Q_OBJECT
public:
    Thema_C(QObject* parent = 0);
    ~Thema_C();

public:

    void SetText(QString text) { _text = text; }
    QString GetText() const { return _text; }

    void SetTrText(QString text) { _translation = text; }
    QString GetTrText() const { return _translation; }

    QVector<Word_C*> GetWords() const { return _words; }

    // Thema_I implementation
public:
    virtual unsigned int GetWordCount() const;
    virtual const Word_C* GetWord(unsigned int index) const;

public:
    bool Read(const QDomElement& element);
    bool Write(QDomElement& element);
    void ClearWords();

private:
    QString _text;
    QString _translation;
    QVector<Word_C*> _words;

private:
    friend class ThemaBuilder_C;
};

#endif // THEMA_H
