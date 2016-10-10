#ifndef TIER1EVENTBROWSER_H_
#define TIER1EVENTBROWSER_H_

// ROOT
#include "TGFrame.h"
#include "TGNumberEntry.h"
#include "TRootEmbeddedCanvas.h"

#include "TChain.h"
#include "TEventList.h"

// GERDA
#include "MGTEvent.hh"

class Tier1EventBrowser : public TGMainFrame { 

public:

    Tier1EventBrowser(const TGWindow *p , int w , int h , char* filename );
    ~Tier1EventBrowser();

    void DoDraw( Long_t );
    void IncrementAndDraw();
    void DecrementAndDraw();
    void RetrieveAndDraw();

    ClassDef(Tier1EventBrowser , 0);

private:

    // data handling
    TChain      *fTree;
    MGTEvent    *fEvent;
    TEventList  *fSelectionList;
    int         fCurrEvent;
    int         fTotEvents;

    // Graphic objects
    TGMainFrame         *fMain;
    TGHorizontalFrame   *fButtonsFrame;
    TRootEmbeddedCanvas *fEmbCanvas;
    TGTextButton        *fPrev;
    TGTextButton        *fNext;
    TGTextButton        *fExit;
    TGNumberEntryField  *fNumberEntry;
    
    // copy/assignement protection
    Tier1EventBrowser           (const Tier1EventBrowser& x);
    Tier1EventBrowser& operator=(const Tier1EventBrowser& x);
};

#endif
