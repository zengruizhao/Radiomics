#ifndef __ccipdComboBoxDelegate_h
#define __ccipdComboBoxDelegate_h



//////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning( disable: 4503 )
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QItemDelegate>

// boost includes
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * Based on http://bit.ly/VhdO6h
 */
class ComboBoxDelegate : public QItemDelegate
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef ComboBoxDelegate Self;
  typedef QItemDelegate    Superclass;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  ComboBoxDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~ComboBoxDelegate();                    ///< Destructor

  QWidget * createEditor(
          QWidget              * parent,
    const QStyleOptionViewItem & option,
    const QModelIndex          & index) const;

  void setEditorData( QWidget * editor, const QModelIndex & index) const;

  void setModelData(
          QWidget            * editor,
          QAbstractItemModel * model,
    const QModelIndex        & index ) const;

  void updateEditorGeometry(
          QWidget              * editor,
    const QStyleOptionViewItem & option,
    const QModelIndex          & index ) const;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  /// What is the type of item we will insert into the combo box?
  typedef std::string ComboItemType;

  /// Make a collection, like std::list but which doesn't allow duplicates.
  struct sequence{};
  typedef boost::multi_index_container<
    ComboItemType,
    boost::multi_index::indexed_by<
      boost::multi_index::sequenced<                    // preserve insertion order
        boost::multi_index::tag< sequence > >,          // name this index
      boost::multi_index::hashed_unique<                // don't allow duplicates
        boost::multi_index::identity< ComboItemType > > // hash the item
    >
  > UniqueItemsCollection;

  /// Use UniqueItemsSequence & sequence = uniqueItems.get< sequence >();
  typedef UniqueItemsCollection::index< sequence >::type UniqueItemsSequence;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Child classes must provide the
   * list of unique items to insert
   * into the combo box.
   * 
   * This type is used so that
   * we will preserve the order
   * of the items in this collection.
   */
  virtual UniqueItemsCollection GetItemsForComboBox() const = 0;

//////////////////////////////////////////////////////////////////////////////////////////



}; // class ComboBoxDelegate
} // namespace ccipd
#endif //__ccipdComboBoxDelegate_h
