#include "my_tinybind.h"

#include <stdio.h>
#include <vector>
#include <list>
#include <istream>
#include <ostream>

struct MyData
{
  int i;
  double d;
  char const * s;
  std::vector<int> vec;
  int iref;

  void setIntvalue( int in ) {
    i = in;
  }
  int intvalue() {
    return i;
  }

  int & getIRef() {
    return iref;
  }
};


TiXml_Binding<MyData> const *
get_tixml_binding( MyData const &, Identity<MyData> )
{
  static Member_TiXml_Binding<MyData> binding;
  if( binding.empty() ) {
    binding.add_member( "ITAG", member(&MyData::i) );
    binding.add_member( "ITAGGETSET", member(&MyData::intvalue, &MyData::setIntvalue) );
    binding.add_member( "DTAG", member(&MyData::d) );
    binding.add_member( "STAG", member(&MyData::s) );
    binding.add_member( "VEC", member(&MyData::vec) );
    binding.add_member( "IREF", member(&MyData::getIRef) );
  }
  return &binding;
}


#if 0
typedef std::vector<int> VecT;
TiXml_Binding<VecT> const *
get_tixml_binding( VecT const & )
{
  static Stl_Container_TiXml_Binding<int, VecT> binding(true, "INT");
  return &binding;
}

typedef std::list<MyData> VecT2;
TiXml_Binding<VecT2> const *
get_tixml_binding( VecT2 const & )
{
  static Stl_Container_TiXml_Binding<MyData, VecT2> binding(false, "DATAVECENTRY");
  return &binding;
}

#endif


struct MyData2
{
  MyData dataOne;
  std::list<MyData> dataVec;
  int xyz;
  char *str;
  char str_array[32];
};

TiXml_Binding<MyData2> const *
get_tixml_binding( MyData2 const &, Identity<MyData2> )
{
  static Member_TiXml_Binding<MyData2> binding;
  if( binding.empty() ) {
    binding.add_member( "XYZ", member_attribute(&MyData2::xyz ))->set_flags(Member_Optional);
    binding.add_member( "DATAONE", member(&MyData2::dataOne) );
    binding.add_member( "DATAVEC", member_peer(&MyData2::dataVec) );
    binding.add_member ("STR", member(&MyData2::str))->set_flags (Member_Optional);
    binding.add_member ("STR_Array", member_str(&MyData2::str_array))->set_flags (Member_Optional);
  }
  return &binding;
}

int main()
{
  MyData testData;
  testData.i = 100;
  testData.iref = 52;
  testData.d = 42.3;
  testData.s = "";
  testData.vec.push_back(1);
  testData.vec.push_back(1);
  testData.vec.push_back(2);
  testData.vec.push_back(3);
  testData.vec.push_back(5);
  printf( "\nTestData: %d %d %g %s %d\n", testData.i, testData.iref, testData.d, testData.s, testData.vec[0] );
  TiXmlElement test("TESTDATA");
  bind_to_xml( &test, testData );
  std::string str;
  std::ostringstream o_str(str);
  str << test;
  printf("%s\n", str.c_str());
  test.Print( stdout, 0 );
  printf("-----------------------\n");
  MyData testData2;

  TiXmlDocument test_doc;
  std::istringstream i_str(str);
  i_str >> test_doc;
  TiXmlElement *test_1 = test_doc.FirstChildElement();
  test_1->Print(stdout, 0);
  printf("-----------------------\n");
  bind_from_xml( *test_1, &testData2 );
  printf( "\nTestData2: %d %d %g %s %d\n", testData2.i, testData2.iref, testData2.d, testData2.s, testData2.vec[0] );

  TiXmlElement testAgain("AGAIN");
  bind_to_xml( &testAgain, testData2 );
  testAgain.Print( stdout, 0 );

  MyData2 testData3;
  testData3.xyz = 10342;
  testData3.dataOne = testData2;
  testData3.dataVec.push_back(testData2);
  testData3.dataVec.push_back(testData2);
  testData3.str = "111123333333333333333333333";
  testData3.str_array[0] = '\0';
  printf( "\nTestData3: %i %i %g %s %s\n", testData3.xyz, testData3.dataOne.i,
    testData3.dataOne.d, testData3.dataOne.s, testData3.dataVec.front().s );

  TiXmlElement test2("TEST2");
  bind_to_xml( &test2, testData3 );
  test2.Print( stdout, 0 );
  MyData2 testData4;
  bind_from_xml( test2, &testData4 );
  printf( "\nTestData4: %i %i %g %s %s\n", testData4.xyz, testData4.dataOne.i, testData4.dataOne.d,
    testData4.dataOne.s, testData4.dataVec.front().s );

  MyData2 testData5;
  test2.SetAttribute("XYZ", "");
  testData5.xyz = 0;
  bind_from_xml( test2, &testData5 );
  printf( "\nTestData5: %i %i %g %s %s\n", testData5.xyz, testData5.dataOne.i, testData5.dataOne.d,
    testData5.dataOne.s, testData5.dataVec.front().s );
  return 0;
}
