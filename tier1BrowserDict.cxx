// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIluigidIcodedIGERDAanalysisdItier1BrowserDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "tier1Browser.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_Tier1EventBrowser(void *p);
   static void deleteArray_Tier1EventBrowser(void *p);
   static void destruct_Tier1EventBrowser(void *p);
   static void streamer_Tier1EventBrowser(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tier1EventBrowser*)
   {
      ::Tier1EventBrowser *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Tier1EventBrowser >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Tier1EventBrowser", ::Tier1EventBrowser::Class_Version(), "tier1Browser.h", 15,
                  typeid(::Tier1EventBrowser), DefineBehavior(ptr, ptr),
                  &::Tier1EventBrowser::Dictionary, isa_proxy, 16,
                  sizeof(::Tier1EventBrowser) );
      instance.SetDelete(&delete_Tier1EventBrowser);
      instance.SetDeleteArray(&deleteArray_Tier1EventBrowser);
      instance.SetDestructor(&destruct_Tier1EventBrowser);
      instance.SetStreamerFunc(&streamer_Tier1EventBrowser);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tier1EventBrowser*)
   {
      return GenerateInitInstanceLocal((::Tier1EventBrowser*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tier1EventBrowser*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Tier1EventBrowser::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Tier1EventBrowser::Class_Name()
{
   return "Tier1EventBrowser";
}

//______________________________________________________________________________
const char *Tier1EventBrowser::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tier1EventBrowser*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Tier1EventBrowser::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tier1EventBrowser*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Tier1EventBrowser::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tier1EventBrowser*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Tier1EventBrowser::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tier1EventBrowser*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Tier1EventBrowser::Streamer(TBuffer &R__b)
{
   // Stream an object of class Tier1EventBrowser.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Tier1EventBrowser(void *p) {
      delete ((::Tier1EventBrowser*)p);
   }
   static void deleteArray_Tier1EventBrowser(void *p) {
      delete [] ((::Tier1EventBrowser*)p);
   }
   static void destruct_Tier1EventBrowser(void *p) {
      typedef ::Tier1EventBrowser current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Tier1EventBrowser(TBuffer &buf, void *obj) {
      ((::Tier1EventBrowser*)obj)->::Tier1EventBrowser::Streamer(buf);
   }
} // end of namespace ROOT for class ::Tier1EventBrowser

namespace {
  void TriggerDictionaryInitialization_tier1BrowserDict_Impl() {
    static const char* headers[] = {
"tier1Browser.h",
0
    };
    static const char* includePaths[] = {
"/home/luigi/programs/MGDO/install/include/mgdo",
"/home/luigi/programs/MGDO/install/include/tam",
"/home/luigi/programs/clhep_v2.1.3.1/include",
"/home/luigi/programs/root_v6.03.02/include",
"/home/luigi/code/GERDAanalysis/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$tier1Browser.h")))  Tier1EventBrowser;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "tier1Browser.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Tier1EventBrowser", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("tier1BrowserDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_tier1BrowserDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_tier1BrowserDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_tier1BrowserDict() {
  TriggerDictionaryInitialization_tier1BrowserDict_Impl();
}
