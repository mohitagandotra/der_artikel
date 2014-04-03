#ifndef WORD_H
#define WORD_H
#include "article.h"
#include <QDomElement>
#include <QObject>

class Word_C : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString word_text READ GetWordText CONSTANT)
    Q_PROPERTY(Article_C::Artikel user_article READ GetUserArtikel NOTIFY userArticleChanged)
    Q_PROPERTY(Article_C::Artikel article READ GetArtikel CONSTANT)

public:
    Word_C(QObject* parent=0);

public:
    void SetUserArtikel(Article_C::Artikel article);
    Article_C::Artikel GetUserArtikel() const { return _user_artikel; }

    Article_C::Artikel GetArtikel() const { return _artikel; }

    QString GetWordText() const { return _text; }

    QString GetDescription() const  { return _description; }

public:
    bool Read(const QDomElement& element);
    bool Write(QDomElement& element);
    bool IsCorrect() const;

signals:
    void userArticleChanged();

private:
    QString _text;
    QString _description;
    Article_C::Artikel _artikel;
    Article_C::Artikel _user_artikel;

private:
    friend class ThemaBuilder_C;
};
#endif // WORD_H
