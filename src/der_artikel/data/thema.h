#ifndef THEMA_H
#define THEMA_H
#include <QObject>
#include <QDomElement>
#include <QVector>
#include <QUrl>
#include "word.h"
#include "thema_interface.h"

class Thema_C : public QObject, public Thema_I
{

    Q_OBJECT
    Q_PROPERTY(bool selected READ Selected WRITE SetSelected)


public:
    Thema_C(QObject* parent = 0);
    ~Thema_C();

public:

    void SetText(QString text) { _text = text; }
    QString GetText() const { return _text; }

    void SetTrText(QString text) { _translation = text; }
    QString GetTrText() const { return _translation; }

    void SetFilePath(QString file_path);

    QVector<Word_C*> GetWords() const { return _words; }

    bool Selected() const { return _selected; }
    void SetSelected(bool selected);

    const QUrl& GetIconUrl() const { return _icon_url; }

    // Thema_I implementation
public:
    virtual unsigned int GetWordCount() const;
    virtual const Word_C* GetWord(unsigned int index) const;

public:
    bool Read(const QDomElement& element);
    bool Write(QDomElement& element);
    void ClearWords();

signals:
    void selectionChanged();

private:
    QString _text;
    QString _translation;
    QString _file_path;
    QVector<Word_C*> _words;
    bool _defered_read;
    double _3rd_last_score;
    double _2nd_last_score;
    double _last_score;
    double _played_count;
    bool _selected;
    QUrl _icon_url;


private:
    friend class ThemaBuilder_C;
};

#endif // THEMA_H
