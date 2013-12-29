#ifndef PAGE_I_H
#define PAGE_I_H

#include "manager.h"
#include <QList>
#include <QString>

/** @brief Interface class for a Page entity. \ref CPageManager manages the pages using this interface.
 *
 */
class Page_I
{
public:
    Page_I() {}
    virtual ~Page_I() {}

public:
    /********************************************************************************/
    /** @brief Returns the page id of the page.
    *   @sa pageId
    *   @return Manager_C::PageId_TP Page id of the page
    *********************************************************************************/
    virtual Manager_C::PageId_TP pageId() const = 0;

    /********************************************************************************/
    /** @brief \ref CPageManger calls this method to decide whether preconditions are
     *  met to enter this page. Implement this method to define the preconditions for
     *  the corresponding page.
    *   @sa canEnter
    *   @return bool True if page can be switched. False otherwise
    *********************************************************************************/
    virtual bool canEnter() const = 0;

    /********************************************************************************/
    /** @brief \ref CPageManger calls this method to just before entering the page.
     *  Implement this method to write initialization code for the corresponding page.
    *   @sa enter
    *********************************************************************************/
    virtual void enter() = 0;

    /********************************************************************************/
    /** @brief \ref CPageManger calls this method to decide whether conditions are
     *  met to leave this page. Implement this method to define the conditions for
     *  the corresponding page.
    *   @sa canLeave
    *   @return bool True if page can be switched off. False otherwise
    *********************************************************************************/
    virtual bool canLeave() const = 0;

    /********************************************************************************/
    /** @brief \ref CPageManger calls this method to just after leaving the page.
     *  Implement this method to write clean up code for the corresponding page.
    *   @sa leave
    *********************************************************************************/
    virtual void leave() = 0;
};

#endif // IPAGE_H
