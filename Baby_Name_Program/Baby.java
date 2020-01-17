public class Baby {

  String name;
  int femaleFreq;
  int maleFreq;
  int femaleRank;
  int maleRank;

  public Baby(String initName, int initFemaleFreq, int initMaleFreq, int initFemaleRank, int initMaleRank) {
    name = initName;
    femaleFreq = initFemaleFreq;
    maleFreq = initMaleFreq;
    femaleRank = initFemaleRank;
    maleRank = initMaleRank;

  }
  public Baby() {
    name = "";
    femaleFreq = 0;
    maleFreq = 0;
    femaleRank = 0;
    maleRank = 0;
  }
  public Baby(Baby baby) {
    name = baby.name;
    femaleFreq = baby.femaleFreq;
    maleFreq = baby.maleFreq;
    femaleRank = baby.femaleRank;
    maleRank = baby.maleRank;
  }
}
