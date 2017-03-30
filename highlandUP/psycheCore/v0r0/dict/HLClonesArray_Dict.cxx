//
// File generated by /home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04/amd64_linux26/bin/rootcint at Thu Mar 30 01:23:54 2017

// Do NOT change. Changes will be lost next time file is generated
//

#define R__DICTIONARY_FILENAME dOdOdIdictdIHLClonesArray_Dict
#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "HLClonesArray_Dict.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
      #if !(defined(R__ACCESS_IN_SYMBOL) || defined(R__USE_SHADOW_CLASS))
      typedef ::HLClonesArray HLClonesArray;
      #else
      class HLClonesArray  {
         public:
         //friend XX;
         // To force the creation of a virtual table, throw just in case.
         virtual ~HLClonesArray() throw() {};
         ::TChain* _chain; //
         int _NObjects; //
         ::TClonesArray* _objects; //
         string _tcArrayName; //
         string _treeName; //
         string _fileName; //
      };
      #endif

   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

namespace ROOT {
   void HLClonesArray_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void HLClonesArray_Dictionary();
   static void delete_HLClonesArray(void *p);
   static void deleteArray_HLClonesArray(void *p);
   static void destruct_HLClonesArray(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HLClonesArray*)
   {
      // Make sure the shadow class has the right sizeof
      R__ASSERT(sizeof(::HLClonesArray) == sizeof(::ROOT::Shadow::HLClonesArray));
      ::HLClonesArray *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::HLClonesArray),0);
      static ::ROOT::TGenericClassInfo 
         instance("HLClonesArray", "./../src/HLClonesArray.hxx", 7,
                  typeid(::HLClonesArray), DefineBehavior(ptr, ptr),
                  &HLClonesArray_ShowMembers, &HLClonesArray_Dictionary, isa_proxy, 4,
                  sizeof(::HLClonesArray) );
      instance.SetDelete(&delete_HLClonesArray);
      instance.SetDeleteArray(&deleteArray_HLClonesArray);
      instance.SetDestructor(&destruct_HLClonesArray);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HLClonesArray*)
   {
      return GenerateInitInstanceLocal((::HLClonesArray*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::HLClonesArray*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void HLClonesArray_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const ::HLClonesArray*)0x0)->GetClass();
   }

} // end of namespace ROOT

//______________________________________________________________________________
namespace ROOT {
   void HLClonesArray_ShowMembers(void *obj, TMemberInspector &R__insp)
   {
      // Inspect the data members of an object of class HLClonesArray.
      typedef ::ROOT::Shadow::HLClonesArray ShadowClass;
      ShadowClass *sobj = (ShadowClass*)obj;
      if (sobj) { } // Dummy usage just in case there is no datamember.

      TClass *R__cl  = ::ROOT::GenerateInitInstanceLocal((const ::HLClonesArray*)0x0)->GetClass();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "*_chain", &sobj->_chain);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_NObjects", &sobj->_NObjects);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "*_objects", &sobj->_objects);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_tcArrayName", (void*)&sobj->_tcArrayName);
      R__insp.InspectMember("string", (void*)&sobj->_tcArrayName, "_tcArrayName.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_treeName", (void*)&sobj->_treeName);
      R__insp.InspectMember("string", (void*)&sobj->_treeName, "_treeName.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_fileName", (void*)&sobj->_fileName);
      R__insp.InspectMember("string", (void*)&sobj->_fileName, "_fileName.", false);
   }

}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HLClonesArray(void *p) {
      delete ((::HLClonesArray*)p);
   }
   static void deleteArray_HLClonesArray(void *p) {
      delete [] ((::HLClonesArray*)p);
   }
   static void destruct_HLClonesArray(void *p) {
      typedef ::HLClonesArray current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::HLClonesArray

/********************************************************
* ../dict/HLClonesArray_Dict.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtableHLClonesArray_Dict();

extern "C" void G__set_cpp_environmentHLClonesArray_Dict() {
  G__cpp_reset_tagtableHLClonesArray_Dict();
}
#include <new>
extern "C" int G__cpp_dllrevHLClonesArray_Dict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* HLClonesArray */
static int G__HLClonesArray_Dict_495_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   HLClonesArray* p = NULL;
   char* gvp = (char*) G__getgvp();
   //m: 4
   if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
     p = new HLClonesArray(
*(string*) libp->para[0].ref, *(string*) libp->para[1].ref
, *(string*) libp->para[2].ref, (const UInt_t) G__int(libp->para[3]));
   } else {
     p = new((void*) gvp) HLClonesArray(
*(string*) libp->para[0].ref, *(string*) libp->para[1].ref
, *(string*) libp->para[2].ref, (const UInt_t) G__int(libp->para[3]));
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HLClonesArray_Dict_495_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((HLClonesArray*) G__getstructoffset())->ReadClonesArray(*(string*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HLClonesArray_Dict_495_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((HLClonesArray*) G__getstructoffset())->WriteClonesArray(*(TTree*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HLClonesArray_Dict_495_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         const Int_t& obj = ((HLClonesArray*) G__getstructoffset())->GetNObjects();
         result7->ref = (long) (&obj);
         G__letint(result7, 'i', (long)obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HLClonesArray_Dict_495_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) ((HLClonesArray*) G__getstructoffset())->GetClonesArray());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HLClonesArray_Dict_495_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         const string& obj = ((const HLClonesArray*) G__getstructoffset())->GetClonesArrayName();
         result7->ref = (long) (&obj);
         result7->obj.i = (long) (&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HLClonesArray_Dict_495_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         const string& obj = ((const HLClonesArray*) G__getstructoffset())->GetTreeName();
         result7->ref = (long) (&obj);
         result7->obj.i = (long) (&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HLClonesArray_Dict_495_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   HLClonesArray* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new HLClonesArray(*(HLClonesArray*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef HLClonesArray G__THLClonesArray;
static int G__HLClonesArray_Dict_495_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 0
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (HLClonesArray*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((HLClonesArray*) (soff+(sizeof(HLClonesArray)*i)))->~G__THLClonesArray();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (HLClonesArray*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((HLClonesArray*) (soff))->~G__THLClonesArray();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__HLClonesArray_Dict_495_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   HLClonesArray* dest = (HLClonesArray*) G__getstructoffset();
   *dest = *(HLClonesArray*) libp->para[0].ref;
   const HLClonesArray& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* HLClonesArray */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncHLClonesArray_Dict {
 public:
  G__Sizep2memfuncHLClonesArray_Dict(): p(&G__Sizep2memfuncHLClonesArray_Dict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncHLClonesArray_Dict::*p)();
};

size_t G__get_sizep2memfuncHLClonesArray_Dict()
{
  G__Sizep2memfuncHLClonesArray_Dict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceHLClonesArray_Dict() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableHLClonesArray_Dict() {

   /* Setting up typedef entry */
   G__search_typename2("Int_t",105,-1,0,-1);
   G__setnewtype(-1,"Signed integer 4 bytes (int)",0);
   G__search_typename2("UInt_t",104,-1,0,-1);
   G__setnewtype(-1,"Unsigned integer 4 bytes (unsigned int)",0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<std::bidirectional_iterator_tag,TObject*,std::ptrdiff_t,const TObject**,const TObject*&>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,std::ptrdiff_t,const TObject**,const TObject*&>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,long>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,long,const TObject**>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<std::string,TObjArray*>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,TObjArray*>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,TObjArray*>",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,TObjArray*,less<string> >",117,G__get_linked_tagnum(&G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* HLClonesArray */
static void G__setup_memvarHLClonesArray(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray));
   { HLClonesArray *p; p=(HLClonesArray*)0x1000; if (p) { }
   G__memvar_setup((void*)0,108,0,0,-1,-1,-1,4,"G__virtualinfo=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_TChain),-1,-1,2,"_chain=",0,(char*)NULL);
   G__memvar_setup((void*)0,105,0,0,-1,G__defined_typename("Int_t"),-1,2,"_NObjects=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_TClonesArray),-1,-1,2,"_objects=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_string),-1,-1,2,"_tcArrayName=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_string),-1,-1,2,"_treeName=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__HLClonesArray_DictLN_string),-1,-1,2,"_fileName=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarHLClonesArray_Dict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncHLClonesArray(void) {
   /* HLClonesArray */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray));
   G__memfunc_setup("HLClonesArray",1271,G__HLClonesArray_Dict_495_0_1, 105, G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray), -1, 0, 4, 1, 1, 0, 
"u 'string' - 11 - tree_name u 'string' - 11 - tcarray_name "
"u 'string' - 11 - class_name h - 'UInt_t' 10 - size", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("ReadClonesArray",1503,G__HLClonesArray_Dict_495_0_2, 121, -1, -1, 0, 1, 1, 1, 0, "u 'string' - 11 - file", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("WriteClonesArray",1646,G__HLClonesArray_Dict_495_0_3, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TTree' - 1 - tree", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("GetNObjects",1080,G__HLClonesArray_Dict_495_0_4, 105, -1, G__defined_typename("Int_t"), 1, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("GetClonesArray",1411,G__HLClonesArray_Dict_495_0_5, 85, G__get_linked_tagnum(&G__HLClonesArray_DictLN_TClonesArray), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("GetClonesArrayName",1796,G__HLClonesArray_Dict_495_0_6, 117, G__get_linked_tagnum(&G__HLClonesArray_DictLN_string), -1, 1, 0, 1, 1, 9, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("GetTreeName",1073,G__HLClonesArray_Dict_495_0_7, 117, G__get_linked_tagnum(&G__HLClonesArray_DictLN_string), -1, 1, 0, 1, 1, 9, "", (char*)NULL, (void*) NULL, 0);
   // automatic copy constructor
   G__memfunc_setup("HLClonesArray", 1271, G__HLClonesArray_Dict_495_0_8, (int) ('i'), G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray), -1, 0, 1, 1, 1, 0, "u 'HLClonesArray' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~HLClonesArray", 1397, G__HLClonesArray_Dict_495_0_9, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 1);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__HLClonesArray_Dict_495_0_10, (int) ('u'), G__get_linked_tagnum(&G__HLClonesArray_DictLN_HLClonesArray), -1, 1, 1, 1, 1, 0, "u 'HLClonesArray' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncHLClonesArray_Dict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalHLClonesArray_Dict() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
}

static void G__cpp_setup_func9() {
}

static void G__cpp_setup_func10() {
}

static void G__cpp_setup_func11() {
}

static void G__cpp_setup_func12() {
}

static void G__cpp_setup_func13() {
}

static void G__cpp_setup_func14() {
}

static void G__cpp_setup_func15() {
}

static void G__cpp_setup_func16() {
}

static void G__cpp_setup_func17() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcHLClonesArray_Dict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
  G__cpp_setup_func9();
  G__cpp_setup_func10();
  G__cpp_setup_func11();
  G__cpp_setup_func12();
  G__cpp_setup_func13();
  G__cpp_setup_func14();
  G__cpp_setup_func15();
  G__cpp_setup_func16();
  G__cpp_setup_func17();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__HLClonesArray_DictLN_string = { "string" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_TClonesArray = { "TClonesArray" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR = { "vector<TVirtualArray*,allocator<TVirtualArray*> >" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR = { "iterator<bidirectional_iterator_tag,TObject*,long,const TObject**,const TObject*&>" , 115 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_TTree = { "TTree" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR = { "map<string,TObjArray*,less<string>,allocator<pair<const string,TObjArray*> > >" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_TChain = { "TChain" , 99 , -1 };
G__linked_taginfo G__HLClonesArray_DictLN_HLClonesArray = { "HLClonesArray" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableHLClonesArray_Dict() {
  G__HLClonesArray_DictLN_string.tagnum = -1 ;
  G__HLClonesArray_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__HLClonesArray_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__HLClonesArray_DictLN_TClonesArray.tagnum = -1 ;
  G__HLClonesArray_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR.tagnum = -1 ;
  G__HLClonesArray_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR.tagnum = -1 ;
  G__HLClonesArray_DictLN_TTree.tagnum = -1 ;
  G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR.tagnum = -1 ;
  G__HLClonesArray_DictLN_TChain.tagnum = -1 ;
  G__HLClonesArray_DictLN_HLClonesArray.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableHLClonesArray_Dict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_string);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_TClonesArray);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_TTree);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR);
   G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_TChain);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__HLClonesArray_DictLN_HLClonesArray),sizeof(HLClonesArray),-1,295936,(char*)NULL,G__setup_memvarHLClonesArray,G__setup_memfuncHLClonesArray);
}
extern "C" void G__cpp_setupHLClonesArray_Dict(void) {
  G__check_setup_version(30051515,"G__cpp_setupHLClonesArray_Dict()");
  G__set_cpp_environmentHLClonesArray_Dict();
  G__cpp_setup_tagtableHLClonesArray_Dict();

  G__cpp_setup_inheritanceHLClonesArray_Dict();

  G__cpp_setup_typetableHLClonesArray_Dict();

  G__cpp_setup_memvarHLClonesArray_Dict();

  G__cpp_setup_memfuncHLClonesArray_Dict();
  G__cpp_setup_globalHLClonesArray_Dict();
  G__cpp_setup_funcHLClonesArray_Dict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncHLClonesArray_Dict();
  return;
}
class G__cpp_setup_initHLClonesArray_Dict {
  public:
    G__cpp_setup_initHLClonesArray_Dict() { G__add_setup_func("HLClonesArray_Dict",(G__incsetup)(&G__cpp_setupHLClonesArray_Dict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initHLClonesArray_Dict() { G__remove_setup_func("HLClonesArray_Dict"); }
};
G__cpp_setup_initHLClonesArray_Dict G__cpp_setup_initializerHLClonesArray_Dict;

