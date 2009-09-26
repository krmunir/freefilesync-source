#ifndef GRIDVIEW_H_INCLUDED
#define GRIDVIEW_H_INCLUDED

#include "../fileHierarchy.h"


namespace FreeFileSync
{
    //gui view of FolderComparison
    class GridView
    {
    public:
        //direct data access via row number
        const FileSystemObject* getObject(unsigned int row) const;  //returns NULL if object is not found; logarithmic complexity
        FileSystemObject* getObject(unsigned int row);              //
        unsigned int rowsOnView() const; //only the currently visible elements
        unsigned int rowsTotal() const;  //total number of rows available

        //get references to FileSystemObject: no NULL-check needed! Everything's bound.
        void getAllFileRef(const std::set<unsigned int>& guiRows, std::vector<FileSystemObject*>& output);

        struct StatusCmpResult
        {
            StatusCmpResult();

            bool existsLeftOnly;
            bool existsRightOnly;
            bool existsLeftNewer;
            bool existsRightNewer;
            bool existsDifferent;
            bool existsEqual;
            bool existsConflict;

            unsigned int filesOnLeftView;
            unsigned int foldersOnLeftView;
            unsigned int filesOnRightView;
            unsigned int foldersOnRightView;

            wxULongLong filesizeLeftView;
            wxULongLong filesizeRightView;
        };

        //comparison results view
        StatusCmpResult updateCmpResult(bool hideFiltered,
                                        bool leftOnlyFilesActive,
                                        bool rightOnlyFilesActive,
                                        bool leftNewerFilesActive,
                                        bool rightNewerFilesActive,
                                        bool differentFilesActive,
                                        bool equalFilesActive,
                                        bool conflictFilesActive);

        struct StatusSyncPreview
        {
            StatusSyncPreview();

            bool existsSyncCreateLeft;
            bool existsSyncCreateRight;
            bool existsSyncDeleteLeft;
            bool existsSyncDeleteRight;
            bool existsSyncDirLeft;
            bool existsSyncDirRight;
            bool existsSyncDirNone;
            bool existsConflict;

            unsigned int filesOnLeftView;
            unsigned int foldersOnLeftView;
            unsigned int filesOnRightView;
            unsigned int foldersOnRightView;

            wxULongLong filesizeLeftView;
            wxULongLong filesizeRightView;
        };

        //synchronization preview
        StatusSyncPreview updateSyncPreview(bool hideFiltered,
                                            bool syncCreateLeftActive,
                                            bool syncCreateRightActive,
                                            bool syncDeleteLeftActive,
                                            bool syncDeleteRightActive,
                                            bool syncDirOverwLeftActive,
                                            bool syncDirOverwRightActive,
                                            bool syncDirNoneActive,
                                            bool conflictFilesActive);



        FolderComparison& getDataTentative();    //get data for operation that does NOT add or reorder rows! (deletion is okay)
        void setData(FolderComparison& newData); //set data, taking ownership: warning std::swap() is used!!!
        void removeInvalidRows();                 //remove rows that have been deleted meanwhile: call after manual deletion and synchronization!
        void clearAllRows();                      //clears everything

        //sorting...
        enum SortType
        {
            SORT_BY_REL_NAME,
            SORT_BY_FILENAME,
            SORT_BY_FILESIZE,
            SORT_BY_DATE,
            SORT_BY_CMP_RESULT,
            SORT_BY_DIRECTORY,
            SORT_BY_SYNC_DIRECTION
        };

        void sortView(const SortType type, const bool onLeft, const bool ascending); //always call this method for sorting, never sort externally!

    private:
        class SerializeHierarchy;

        struct RefIndex
        {
            RefIndex(unsigned int folderInd, FileSystemObject::ObjectID id) :
                    folderIndex(folderInd),
                    objId(id) {}
            unsigned int folderIndex;
            FileSystemObject::ObjectID objId;
        };

        FileSystemObject* getReferencedRow(const RefIndex ref); //returns NULL if not found
        const FileSystemObject* getReferencedRow(const RefIndex ref) const; //returns NULL if not found
        bool isInvalidRow(const RefIndex& ref) const;


        std::vector<RefIndex> viewRef;  //partial view on sortedRef
        //              |
        //              | (update...)
        //             \|/
        std::vector<RefIndex> sortedRef; //equivalent to folerCmp, but may be sorted
        //              |
        //              | (setData)
        //             \|/
        FolderComparison folderCmp; //actual comparison data: owned by GridView!


        //sorting classes
        template <bool ascending>
        class SortByDirectory;

        template <bool ascending, SelectedSide side>
        class SortByRelName;

        template <bool ascending, SelectedSide side>
        class SortByFileName;

        template <bool ascending, SelectedSide side>
        class SortByFileSize;

        template <bool ascending, SelectedSide side>
        class SortByDate;

        template <bool ascending>
        class SortByCmpResult;

        template <bool ascending>
        class SortBySyncDirection;
    };










//############################################################################
//inline implementation

    inline
    const FileSystemObject* GridView::getObject(unsigned int row) const
    {
        if (row < rowsOnView())
            return getReferencedRow(viewRef[row]);
        else
            return NULL;
    }

    inline
    FileSystemObject* GridView::getObject(unsigned int row)
    {
        //code re-use of const method: see Meyers Effective C++
        return const_cast<FileSystemObject*>(static_cast<const GridView&>(*this).getObject(row));
    }


    inline
    unsigned int GridView::rowsOnView() const
    {
        return viewRef.size();
    }


    inline
    FolderComparison& GridView::getDataTentative()
    {
        return folderCmp;
    }

    inline
    unsigned int GridView::rowsTotal() const //total number of rows available
    {
        return sortedRef.size();
    }


    inline
    const FreeFileSync::FileSystemObject* GridView::getReferencedRow(const RefIndex ref) const
    {
        return folderCmp[ref.folderIndex].retrieveById(ref.objId);
    }


    inline
    FreeFileSync::FileSystemObject* GridView::getReferencedRow(const RefIndex ref)
    {
        //code re-use of const method: see Meyers Effective C++
        return const_cast<FileSystemObject*>(static_cast<const GridView&>(*this).getReferencedRow(ref));
    }
}


#endif // GRIDVIEW_H_INCLUDED