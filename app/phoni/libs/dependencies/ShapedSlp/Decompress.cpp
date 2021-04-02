#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include "cmdline.h"
#include "Common.hpp"
#include "PlainSlp.hpp"
#include "PoSlp.hpp"
#include "ShapedSlp_Status.hpp"
#include "ShapedSlp.hpp"
#include "ShapedSlpV2.hpp"
#include "SelfShapedSlp.hpp"
#include "SelfShapedSlpV2.hpp"
#include "DirectAccessibleGammaCode.hpp"
#include "IncBitLenCode.hpp"
#include "FixedBitLenCode.hpp"
#include "SelectType.hpp"
#include "VlcVec.hpp"

using namespace std;

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

using var_t = uint32_t;

template<class SlpT>
void measure
(
 std::string in,
 std::string out
) {
  SlpT slp;

  auto start = timer::now();
  ifstream fs(in);
  slp.load(fs);
  auto stop = timer::now();
  cout << "time to load (ms): " << duration_cast<milliseconds>(stop-start).count() << endl;
  // slp.printStatus();
  std::unique_ptr<char[]> str(new char[slp.getLen()]);
  slp.expandSubstr(0, slp.getLen(), str.get());
  ofstream os(out);
  os << str.get();
}


int main(int argc, char* argv[])
{
  using Fblc = FixedBitLenCode<>;
  using SelSd = SelectSdvec<>;
  using SelMcl = SelectMcl<>;
  using DagcSd = DirectAccessibleGammaCode<SelSd>;
  using DagcMcl = DirectAccessibleGammaCode<SelMcl>;
  using Vlc64 = VlcVec<sdsl::coder::elias_delta, 64>;
  using Vlc128 = VlcVec<sdsl::coder::elias_delta, 128>;
  using funcs_type = map<string,
                         void(*)
                         (
                          std::string in, std::string out
                          )>;
  funcs_type funcs;

  //// PlainSlp
  funcs.insert(make_pair("PlainSlp_FblcFblc", measure<PlainSlp<var_t, Fblc, Fblc>>));
  funcs.insert(make_pair("PlainSlp_IblcFblc", measure<PlainSlp<var_t, IncBitLenCode, Fblc>>));
  funcs.insert(make_pair("PlainSlp_32Fblc", measure<PlainSlp<var_t, FixedBitLenCode<32>, Fblc>>));

  //// PoSlp: Post-order SLP
  //// Sometimes PoSlp_Sd is better than PoSlp_Iblc
  funcs.insert(make_pair("PoSlp_Iblc", measure<PoSlp<var_t, IncBitLenCode>>));
  funcs.insert(make_pair("PoSlp_Sd", measure<PoSlp<var_t, DagcSd>>));
  // funcs.insert(make_pair("PoSlp_Mcl", measure<PoSlp<var_t, DagcMcl>>));

  //// ShapedSlp: plain implementation of slp encoding that utilizes shape-tree grammar
  //// Since bit length to represent slp element is small, SelMcl is good for them.
  //// For stg and bal element, SelSd is better
  funcs.insert(make_pair("ShapedSlp_SdMclSd_SdMcl", measure<ShapedSlp<var_t, DagcSd, DagcMcl, DagcSd, SelSd, SelMcl>>));
  funcs.insert(make_pair("ShapedSlp_SdSdSd_SdMcl", measure<ShapedSlp<var_t, DagcSd, DagcSd, DagcSd, SelSd, SelMcl>>));

  //// ShapedSlpV2: all vlc vectors are merged into one.
  //// Generally encoding size gets worse than ShapedSlp_SdMclSd_SdMcl because
  //// - Since bit length to represnet stg and bal element is large, DagcSd is a good choice.
  //// - On the other hand, bit size to represent slp element is significantly small, and so SelMcl should be used
  funcs.insert(make_pair("ShapedSlpV2_Sd_SdMcl", measure<ShapedSlpV2<var_t, DagcSd, SelSd, SelMcl>>));
  // funcs.insert(make_pair("ShapedSlpV2_SdSdSd", measure<ShapedSlp<var_t, DagcSd, SelSd, SelSd>>));
  // funcs.insert(make_pair("ShapedSlpV2_SdMclMcl", measure<ShapedSlp<var_t, DagcSd, SelMcl, SelMcl>>));
  // funcs.insert(make_pair("ShapedSlpV2_Vlc128SdSd", measure<ShapedSlp<var_t, Vlc128, SelSd, SelSd>>));

  //// SelfShapedSlp: ShapedSlp that does not use shape-tree grammar
  funcs.insert(make_pair("SelfShapedSlp_SdSd_Sd", measure<SelfShapedSlp<var_t, DagcSd, DagcSd, SelSd>>));
  funcs.insert(make_pair("SelfShapedSlp_SdSd_Mcl", measure<SelfShapedSlp<var_t, DagcSd, DagcSd, SelMcl>>));
  // funcs.insert(make_pair("SelfShapedSlp_MclMcl_Sd", measure<SelfShapedSlp<var_t, DagcMcl, DagcMcl, SelSd>>));
  // funcs.insert(make_pair("SelfShapedSlp_SdMcl_Sd", measure<SelfShapedSlp<var_t, DagcSd, DagcMcl, SelSd>>));

  //// SelfShapedSlpV2:
  //// attempted to asign smaller offsets to frequent variables by giving special seats for hi-frequent ones
  funcs.insert(make_pair("SelfShapedSlpV2_SdSd_Sd", measure<SelfShapedSlpV2<var_t, DagcSd, DagcSd, SelSd>>));
  // funcs.insert(make_pair("SelfShapedSlpV2_SdSd_Mcl", measure<SelfShapedSlpV2<var_t, DagcSd, DagcSd, SelMcl>>));

  string methodList;
  for (auto itr = funcs.begin(); itr != funcs.end(); ++itr) {
    methodList += itr->first + ". ";
  }


  cmdline::parser parser;
  parser.add<string>("input", 'i', "input file name in which ShapedSlp data structure is written.", true);
  parser.add<string>("output", 'o', "the decompressed file.", true);
  parser.add<string>("encoding", 'e', "encoding: " + methodList, true);


  parser.add<bool>("dummy_flag", 0, "this is dummy flag to prevent that optimization deletes codes", false, false);
  parser.parse_check(argc, argv);
  const string in = parser.get<string>("input");
  const string out = parser.get<string>("output");
  const string encoding = parser.get<string>("encoding");
  const bool dummy_flag = parser.get<bool>("dummy_flag");



  if (encoding.compare("All") == 0) {
    for (auto itr = funcs.begin(); itr != funcs.end(); ++itr) {
      cout << itr->first << ": BEGIN" << std::endl;
      itr->second(in + itr->first, out);
      cout << itr->first << ": END" << std::endl;
    }
  } else {
    auto itr = funcs.find(encoding);
    if (itr != funcs.end()) {
      cout << itr->first << ": BEGIN" << std::endl;
      itr->second(in, out);
      cout << itr->first << ": END" << std::endl;
    } else {
      cerr << "error: specify a valid encoding name in " + methodList << endl;
      exit(1);
    }
  }


  return 0;
}
