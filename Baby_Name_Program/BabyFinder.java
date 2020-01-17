
import java.util.*;
import java.io.*;
import java.util.Scanner;
import java.text.*;


public class BabyFinder {

  public static Baby[] topMale = new Baby[10];
  public static Baby[] topFemale = new Baby[10];
  public static int numFemale = 0;
  public static int numMale = 0;

  public static Baby[] uniqueMale = new Baby[5];
  public static Baby[] uniqueFemale = new Baby[5];

  public static int totalBabies = 0;

  public static HashMap<String, Baby> storeBabies(String babyFile) {
    HashMap<String, Baby> babies = new HashMap<String, Baby>();

    try {
      Scanner input = new Scanner(new File(babyFile));
      int femaleRank = 0;
      int maleRank = 0;

      int i = 0;

      while (input.hasNextLine()) {
        String[] babyArray = input.nextLine().split(",");
        //Baby baby = new Baby(babyArray[0], babyArray[1], Integer.parseInt(babyArray[2]), 0, femaleCount, maleCount);

        if (babyArray[1].equals("F")) {

          femaleRank++;
          Baby baby = new Baby(babyArray[0], "yes", Integer.parseInt(babyArray[2]), 0, femaleRank, 0);
          babies.put(baby.name, baby);
          numFemale = numFemale + baby.femaleFreq;
          totalBabies = totalBabies + baby.femaleFreq;

          if (femaleRank <= 10) {
            topFemale[femaleRank-1] = baby;
          }

        } else {

            if (babies.containsKey(babyArray[0])) {
              maleRank++;
              Baby baby = new Baby(babyArray[0], "no", babies.get(babyArray[0]).femaleFreq, Integer.parseInt(babyArray[2]), babies.get(babyArray[0]).femaleRank, maleRank);
              babies.put(babyArray[0], baby);
              numMale = numMale + baby.maleFreq;
              totalBabies = totalBabies + baby.maleFreq;

              if (baby.maleRank <= 10) {
                topMale[maleRank-1] = baby;
              }
            } else {
              maleRank++;

              Baby baby = new Baby(babyArray[0], "yes", 0, Integer.parseInt(babyArray[2]), 0, maleRank);
              babies.put(babyArray[0], baby);
              numMale = numMale + baby.maleFreq;
              totalBabies = totalBabies + baby.maleFreq;

              if (i < 5) {
                uniqueMale[i] = baby;
                i++;
              }

              if (baby.maleRank <= 10) {
                topMale[maleRank-1] = baby;
              }
            }

        }

      }

    }
    catch (FileNotFoundException e) {
      System.out.println("File does not exist");
      System.exit(0);

    }

    return babies;

  }

  public static void SearchName(HashMap<String, Baby> babyMap, String name) {

    Baby baby = babyMap.get(name);

    int maleNum = baby.maleFreq;
    int femaleNum = baby.femaleFreq;
    int maleRank = baby.maleRank;
    int femaleRank = baby.femaleRank;

    System.out.println("Selected Name: " + name);
    System.out.println("Male Occurrences: " + maleNum + ". " + "Male Rank: " + maleRank);

    System.out.println("Female Occurrences: " + femaleNum + ". " + "Female Rank: " + femaleRank);


  }

  public static void MostPopularNames(HashMap<String, Baby> babyMap) {

    System.out.println("Top 10 Female Baby Names:");

    DecimalFormat df = new DecimalFormat("#.##");

    // Prints the top 10 female baby Names
    for (Baby baby: topFemale) {
      float percent = (float)baby.femaleFreq * 100 / numFemale;

      System.out.println(baby.name + "          " + baby.femaleFreq + "   " +  df.format(percent) + "%");
    }

    System.out.println("Top 10 Male Baby Names:");
    for (Baby baby: topMale) {
      float percent = (float)baby.maleFreq * 100 / numMale;
      System.out.println(baby.name + "           " + baby.maleFreq + "   " + df.format(percent) + "%");
    }


  }


  public static void UniqueNames(HashMap<String, Baby> babyMap) {


    Collection coll = babyMap.values();
    Iterator itr = coll.iterator();

    int i = 0;
    while (itr.hasNext() && i < 5) {
      Baby baby = (Baby)itr.next();

      if (baby.maleFreq == 0) {
        uniqueFemale[i] = baby;
        i++;
      }
    }

    System.out.println("Unique Male Names");

    for (Baby baby: uniqueMale) {
      System.out.println(baby.name);
    }

    System.out.println("Unique Female Names");

    for (Baby baby: uniqueFemale) {
      System.out.println(baby.name);
    }





  }

  public static void DisplayNames(HashMap<String, Baby> babyMap) {

    String[] sortedMap = new String[babyMap.size()];
    int i = 0;

    Iterator itr = babyMap.values().iterator();

    while (itr.hasNext()) {
      Baby baby = (Baby)itr.next();

      sortedMap[i] = baby.name;
      i++;
    }

    Arrays.sort(sortedMap);

    for (String name: sortedMap) {

      DecimalFormat df = new DecimalFormat("#.####");

      Baby baby = babyMap.get(name);
      int sum = baby.maleFreq + baby.femaleFreq;
      float percent = (float)sum * 100 / totalBabies;
      System.out.println(name + "  " + sum + "    " +  df.format(percent) + "%");
    }
    System.out.println(totalBabies);

      //int maleAfemale = baby.maleFreq + baby.femaleFreq;

      //System.out.println(baby.name + "    " + maleAfemale);






  }








}
