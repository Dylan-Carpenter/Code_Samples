
import java.util.*;
import java.io.*;
import java.util.Scanner;
import java.text.*;


public class BabyHashMap {

  //pretty colors
  public static final String ANSI_RED = "\u001B[35m";
  public static final String ANSI_RESET = "\u001B[0m";
  public static final String ANSI_BLUE = "\u001B[34m";

  public static Baby[] topMale = new Baby[10];
  public static Baby[] topFemale = new Baby[10];
  public static int numFemale = 0;
  public static int numMale = 0;

  public static Baby[] uniqueMale = new Baby[5];
  public static Baby[] uniqueFemale = new Baby[5];

  public static int totalBabies = 0;

  // Stores all of the babies in a hashmap
  public static HashMap<String, Baby> storeBabies(String babyFile) {
    HashMap<String, Baby> babies = new HashMap<String, Baby>();

    try {
      Scanner input = new Scanner(new File(babyFile));
      int femaleRank = 0;
      int maleRank = 0;

      int i = 0;
      int j = 0;

      // Traverses through each line of input from the text file
      while (input.hasNextLine()) {
        String[] babyArray = input.nextLine().split(",");

        // Adds baby if it is a female
        if (babyArray[1].equals("F")) {

          femaleRank++;
          Baby baby = new Baby(babyArray[0], Integer.parseInt(babyArray[2]), 0, femaleRank, 0);
          babies.put(baby.name, baby);
          numFemale = numFemale + baby.femaleFreq;
          totalBabies = totalBabies + baby.femaleFreq;

          // Keeps track of most popular babies
          if (femaleRank <= 10) {
            topFemale[femaleRank-1] = baby;
          }
          // keeps track of unique babies
          if (baby.femaleFreq == 5 && i < 5) {
            uniqueFemale[i] = baby;
            i++;
          }

        } else {

          // Adds male babies
            if (babies.containsKey(babyArray[0])) {
              maleRank++;
              Baby baby = new Baby(babyArray[0], babies.get(babyArray[0]).femaleFreq, Integer.parseInt(babyArray[2]), babies.get(babyArray[0]).femaleRank, maleRank);
              babies.put(babyArray[0], baby);
              numMale = numMale + baby.maleFreq;
              totalBabies = totalBabies + baby.maleFreq;

              // Keeps track of unique males
             if (baby.maleFreq == 5 && j < 5) {
               uniqueMale[j] = baby;
               j++;
             }

              // Keeps track of popular males
              if (baby.maleRank <= 10) {
                topMale[maleRank-1] = baby;
              }
            } else {
              maleRank++;

              Baby baby = new Baby(babyArray[0], 0, Integer.parseInt(babyArray[2]), 0, maleRank);
              babies.put(babyArray[0], baby);
              numMale = numMale + baby.maleFreq;
              totalBabies = totalBabies + baby.maleFreq;

              // Keeps track of unique males
              if (baby.maleFreq == 5 && j < 5) {
                uniqueMale[j] = baby;
                j++;
              }

              // Keeps track of popular males
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

  // Searches for a given name and returns information about that particular name
  public static void SearchName(HashMap<String, Baby> babyMap, String name) {

    Baby baby = babyMap.get(name);

    int maleNum = baby.maleFreq;
    int femaleNum = baby.femaleFreq;
    int maleRank = baby.maleRank;
    int femaleRank = baby.femaleRank;

    //System.out.println("Selected Name: " + name);
    System.out.println(ANSI_BLUE + "Male Occurrences: " + maleNum + " " + "Male Rank: " + maleRank + ANSI_RESET);

    System.out.println(ANSI_RED + "Female Occurrences: " + femaleNum + " " + "Female Rank: " + femaleRank + "\n\n\n\n\n\n" + ANSI_RESET);

  }

  // Prints out the top ten most popular male and female names as well as the
  // frequencies and percents for each
  public static void MostPopularNames(HashMap<String, Baby> babyMap) {

    System.out.println();
    System.out.println(ANSI_RED + "Top 10 Female Baby Names:" + ANSI_RESET);
    System.out.println();

    DecimalFormat df = new DecimalFormat("#.##");

    // Prints the top 10 female baby Names
    for (Baby baby: topFemale) {
      float percent = (float)baby.femaleFreq * 100 / numFemale;

      System.out.println(ANSI_RED + "Name: " + baby.name + " Frequency: " + baby.femaleFreq + " Percentage " +  df.format(percent) + "%" + ANSI_RESET);
    }

    // Prints top 10 male names
    System.out.println(ANSI_BLUE + "\nTop 10 Male Baby Names:" + ANSI_RESET);
    System.out.println();
    for (Baby baby: topMale) {
      float percent = (float)baby.maleFreq * 100 / numMale;
      System.out.println(ANSI_BLUE + "Name: " + baby.name + " Frequency: " + baby.maleFreq + " Percentage: " + df.format(percent) + "%" + ANSI_RESET);
    }


  }

  // Prints 5 unique female and male names along with
  // their frequencies and percentages
  public static void UniqueNames(HashMap<String, Baby> babyMap) {

        System.out.println(ANSI_BLUE + "Unique Male Names" + ANSI_RESET);

        DecimalFormat df = new DecimalFormat("#.####");

        // Prints unique male names
        for (Baby baby: uniqueMale) {
          float percent = (float)baby.maleFreq * 100 / numMale;
          System.out.println(ANSI_BLUE + "Name: " + baby.name  + " Frequency: " + baby.maleFreq + " Percent: " + df.format(percent) + "%" + ANSI_RESET);
        }

        System.out.println(ANSI_RED + "\nUnique Female Names" + ANSI_RESET);
        // Prints unique female names
        for (Baby baby: uniqueFemale) {
          float percent = (float)baby.femaleFreq * 100 / numFemale;
          System.out.println(ANSI_RED + "Name: " + baby.name + " Frequency: " + baby.femaleFreq + " Percent: " + df.format(percent) + "%" + ANSI_RESET);
        }
  }

  // Displays all of the names in alphabetical order, along with the total amount of babies with that name and the percent of babies with that name
  public static void DisplayNames(HashMap<String, Baby> babyMap) {

    String[] sortedMap = new String[babyMap.size()];
    int i = 0;

    Iterator itr = babyMap.values().iterator();

    // Traverses through each baby in the hashmap
    while (itr.hasNext()) {
      Baby baby = (Baby)itr.next();

      sortedMap[i] = baby.name;
      i++;
    }

    Arrays.sort(sortedMap);

    // Prints out the contents of the sorted array
    for (String name: sortedMap) {

      DecimalFormat df = new DecimalFormat("#.####");

      Baby baby = babyMap.get(name);
      int sum = baby.maleFreq + baby.femaleFreq;
      float percent = (float)sum * 100 / totalBabies;
      System.out.println(name + "  " + sum + "    " +  df.format(percent) + "%");
    }
    System.out.println(totalBabies);

  }








}
