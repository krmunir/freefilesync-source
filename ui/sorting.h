#ifndef SORTING_H_INCLUDED
#define SORTING_H_INCLUDED

#include "../fileHierarchy.h"
#include "../shared/systemConstants.h"
#include "../synchronization.h"
#include "../shared/staticAssert.h"


namespace FreeFileSync
{
inline
int compareString(const Zstring& stringA, const Zstring& stringB)
{
#ifdef FFS_WIN //Windows does NOT distinguish between upper/lower-case
    return stringA.CmpNoCase(stringB);
#elif defined FFS_LINUX //Linux DOES distinguish between upper/lower-case
    return stringA.Cmp(stringB);
#endif
}


template <bool ascending>
struct Compare
{
    template <class T>
    bool isSmallerThan(T a, T b)
    {
        assert_static(sizeof(T) <= 2 * sizeof(int)); //use for comparing (small) INTEGRAL types only!
        return a < b;
    }
};
template <>
struct Compare<false>
{
    template <class T>
    bool isSmallerThan(T a, T b)
    {
        assert_static(sizeof(T) <= 2 * sizeof(int)); //use for comparing (small) INTEGRAL types only!
        return a > b;
    }
};


inline
bool stringSmallerThan(const Zstring& stringA, const Zstring& stringB)
{
    return compareString(stringA, stringB) < 0;
}


template <bool ascending, SelectedSide side>
inline
bool sortByFileName(const FileSystemObject& a, const FileSystemObject& b)
{
    //presort types: first files, then directories then empty rows
    if (a.isEmpty<side>())
        return false;  //empty rows always last
    else if (b.isEmpty<side>())
        return true;  //empty rows always last


    if (dynamic_cast<const DirMapping*>(&a)) //sort directories by relative name
    {
        if (dynamic_cast<const DirMapping*>(&b))
            return stringSmallerThan(a.getRelativeName<side>(), b.getRelativeName<side>());
        else
            return false;
    }
    else
    {
        if (dynamic_cast<const DirMapping*>(&b))
            return true;
        else
            return Compare<ascending>().isSmallerThan(
                       compareString(a.getShortName<side>(), b.getShortName<side>()), 0);
    }
}


template <bool ascending, SelectedSide side>
bool sortByRelativeName(const FileSystemObject& a, const FileSystemObject& b)
{
    if (a.isEmpty<side>())
        return false;  //empty rows always last
    else if (b.isEmpty<side>())
        return true;  //empty rows always last

    const FileMapping* fileObjA = dynamic_cast<const FileMapping*>(&a);
    const Zstring relDirNameA = fileObjA != NULL ?
                                a.getParentRelativeName() : //file
                                a.getRelativeName<side>();  //directory

    const FileMapping* fileObjB = dynamic_cast<const FileMapping*>(&b);
    const Zstring relDirNameB = fileObjB != NULL ?
                                b.getParentRelativeName() : //file
                                b.getRelativeName<side>();  //directory

    //compare relative names without filenames first
    const int rv = compareString(relDirNameA, relDirNameB);
    if (rv != 0)
        return Compare<ascending>().isSmallerThan(rv, 0);
    else //compare the filenames
    {
        if (fileObjB == NULL) //directories shall appear before files
            return false;
        else if (fileObjA == NULL)
            return true;

        return stringSmallerThan(a.getShortName<side>(), b.getShortName<side>());
    }
}


template <bool ascending, SelectedSide side>
inline
bool sortByFileSize(const FileSystemObject& a, const FileSystemObject& b)
{
    if (a.isEmpty<side>())
        return false;  //empty rows always last
    else if (b.isEmpty<side>())
        return true;  //empty rows always last


    const FileMapping* fileObjA = dynamic_cast<const FileMapping*>(&a);
    const FileMapping* fileObjB = dynamic_cast<const FileMapping*>(&b);

    if (fileObjA == NULL)
        return false; //directories last
    else if (fileObjB == NULL)
        return true;  //directories last

    //sortAscending shall result in list beginning with largest files first
    return Compare<!ascending>().isSmallerThan(fileObjA->getFileSize<side>(), fileObjB->getFileSize<side>());
}


template <bool ascending, SelectedSide side>
inline
bool sortByDate(const FileSystemObject& a, const FileSystemObject& b)
{
    if (a.isEmpty<side>())
        return false;  //empty rows always last
    else if (b.isEmpty<side>())
        return true;  //empty rows always last


    const FileMapping* fileObjA = dynamic_cast<const FileMapping*>(&a);
    const FileMapping* fileObjB = dynamic_cast<const FileMapping*>(&b);

    if (fileObjA == NULL)
        return false; //directories last
    else if (fileObjB == NULL)
        return true;  //directories last

    return Compare<!ascending>().isSmallerThan(fileObjA->getLastWriteTime<side>(), fileObjB->getLastWriteTime<side>());
}


template <bool ascending>
inline
bool sortByCmpResult(const FileSystemObject& a, const FileSystemObject& b)
{
    //presort result: equal shall appear at end of list
    if (a.getCategory() == FILE_EQUAL)
        return false;
    if (b.getCategory() == FILE_EQUAL)
        return true;

    return Compare<ascending>().isSmallerThan(a.getCategory(), b.getCategory());
}


template <bool ascending>
inline
bool sortBySyncDirection(const FileSystemObject& a, const FileSystemObject& b)
{
    return Compare<ascending>().isSmallerThan(a.getSyncOperation(), b.getSyncOperation());
}
}

#endif // SORTING_H_INCLUDED
