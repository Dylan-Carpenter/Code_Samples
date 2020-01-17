import java.util.*;
import java.io.*;
import java.util.Scanner;
import java.lang.*;

public class BabyArrayList {

  //pretty colors
  public static final String ANSI_RED = "\u001B[35m";
  public static final String ANSI_RESET = "\u001B[0m";
  public static final String ANSI_BLUE = "\u001B[34m";

//LinkedList public saved
public static ArrayList<Baby> babyBoyz = new ArrayList<Baby>();

public static ArrayList<Baby> babyGrillz = new ArrayList<Baby>();

public static ArrayList<String> girlNames = new ArrayList<String>();

public static ArrayList<String> boyNames = new ArrayList<String>();

public static ArrayList<Baby> uniqueMale  = new ArrayList<Baby>();
public static ArrayList<Baby> uniqueFemale  = new ArrayList<Baby>();


public static int totalMales = 0;
public static int totalGirls = 0;

  //loads up the female babies into babyGrillz and male babies into babyBoyz
  public static ArrayList<Baby> loadGirls(String babyFile){

    try{
      Scanner input = new Scanner(new File(babyFile));
      int maleRankCounter = 1;
      int femaleRankCounter = 1;
      int i = 0;
      int j = 0;

      //goes through the input file
      while (input.hasNext()){
        String line = input.next();
        String [] baby = line.split(",");

        //ads baby if its male
        if(baby[1].equals("M")){
          Baby newBabyBoy = new Baby(baby[0], 0, Integer.parseInt(baby[2]), 0, maleRankCounter);
          babyBoyz.add(newBabyBoy);
          boyNames.add(baby[0]);
          maleRankCounter++;
          totalMales = totalMales + Integer.parseInt(baby[2]);

          if (i < 5 && (Integer.parseInt(baby[2]) == 5)) {
            uniqueMale.add(newBabyBoy);
            i++;
          }
          //adds baby if its a girl
        }else{
          Baby newBabyGril = new Baby(baby[0], Integer.parseInt(baby[2]), 0, femaleRankCounter, 0);
          babyGrillz.add(newBabyGril);
          girlNames.add(baby[0]);
          femaleRankCounter++;
          totalGirls = totalGirls + Integer.parseInt(baby[2]);

          if (j < 5 && (Integer.parseInt(baby[2]) == 5)) {
            uniqueFemale.add(newBabyGril);
            j++;
          }
        }

    }
  }
    catch (FileNotFoundException e){
      System.out.println("File does not exist");
    }

    return babyGrillz;

  }

  //returns the babyBoyz ArrayList
  public static ArrayList<Baby> loadBoys(){
    return babyBoyz;
  }

  //searches for a particular name and prints out the frequencies and ranks
  public static void SearchName(String babyName){

    int femaleBabies = 0;
    int femaleRank = 0;
    int maleBabies = 0;
    int maleRank = 0;

    //iterates through baby girls
    for(int i = 0; i < babyGrillz.size(); i++){
      Baby currentBaby = babyGrillz.get(i);
      if(currentBaby.name.equals(babyName)){
        femaleBabies = currentBaby.femaleFreq;
        femaleRank = currentBaby.femaleRank;
      }
    }

    //iterates through baby boys
    for(int j = 0; j < babyBoyz.size(); j++){
      Baby currentBaby = babyBoyz.get(j);
      if(currentBaby.name.equals(babyName)){
        maleBabies = currentBaby.maleFreq;
        maleRank = currentBaby.maleRank;
      }
    }

    //prints results
    System.out.println(" ");
    System.out.println("Data retrieved: ");
    System.out.println(" ");
    System.out.println(ANSI_RED + "Number of Female Babies: " + femaleBabies + "  " + "Female Popularity Rank: " + femaleRank + ANSI_RESET);
    System.out.println(ANSI_BLUE + "Number of Male Babies: " + maleBabies + "  " + "Male Popularity Rank: " + maleRank + ANSI_RESET);
  }

  //prints out the top ten most popular names from each gender and their frequencies and percentages
  public static void MostPopularName(){

    System.out.println(" ");
    System.out.println("Data retrieved: ");
    System.out.println(" ");
    System.out.println(ANSI_RED + "Females: " + ANSI_RESET);
    System.out.println(" ");

    //grabs the most popular girls
    for(int i = 0; i < 9; i++){
      Baby currentBaby = babyGrillz.get(i);
      float percentage;
      percentage = (float) currentBaby.femaleFreq * 100 / totalGirls;
      System.out.println(ANSI_RED + "Name: " + currentBaby.name + "  Frequency: " + currentBaby.femaleFreq + "  Percentage: " + percentage + "%" + ANSI_RESET);
      System.out.println(" ");
    }

    System.out.println(ANSI_BLUE + "Males: " + ANSI_RESET);
    System.out.println(" ");

    //grabs the most popular boys
    for(int j = 0; j < 9; j++){
      Baby currentBaby = babyBoyz.get(j);
      float percentage;
      percentage = (float) currentBaby.maleFreq * 100/ totalMales;
      System.out.println(ANSI_BLUE + "Name: " + currentBaby.name + "  Frequency: " + currentBaby.maleFreq + "  Percentage: " + percentage + "%" + ANSI_RESET);
      System.out.println(" ");
    }
  }

  //prints out the first names from each gender that have a frequency of 5
  public static void UniqueName(){
    System.out.println(" ");
    System.out.println("Data retrieved: ");
    System.out.println(" ");
    System.out.println(ANSI_RED + "Females: " + ANSI_RESET);
    System.out.println(" ");

    //grabs unique names from female babies
    for(Baby baby: uniqueFemale){
      int femaleFrequency = baby.femaleFreq;
      float girlPercentage = (float) femaleFrequency * 100/ totalGirls;
      System.out.println(ANSI_RED + "Name: " + baby.name + "  Frequency: " + 5 + "  Percentage: " + girlPercentage + "%" + ANSI_RESET);
    }


    System.out.println(" ");
    System.out.println(ANSI_BLUE + "Males: " + ANSI_RESET);
    System.out.println(" ");

    //grabs unique names from male babies
    for(Baby baby: uniqueMale){
      int maleFrequency = baby.maleFreq;
      float boyPercentage = (float) maleFrequency * 100/ totalMales;
      System.out.println(ANSI_BLUE + "Name: " + baby.name + "  Frequency: " + maleFrequency + "  Percentage: " + boyPercentage + "%" + ANSI_RESET);
    }
  }

  //displays all names with their associated info in alphabetic order
  public static void DisplayName(){

        System.out.println(" ");
        System.out.println("Data retrieved: ");
        System.out.println(" ");
        System.out.println("Babies: ");
        System.out.println(" ");

        //grabs all girls names
        for(int i = 0; i < boyNames.size(); i++){
          String name = boyNames.get(i);
          if(girlNames.contains(name)){
            break;
          }else{
            girlNames.add(name);
          }
        }

        //alphabetize girl names
        Collections.sort(girlNames);

        int nameCount = 0;

        //grabs and prints out all babies in order
        while(nameCount < girlNames.size()){

          int femaleFrequency = 0;
          int maleFrequency = 0;
          String name = " ";

          for(int i = 0; i < babyGrillz.size(); i++){
            Baby currentBaby = babyGrillz.get(i);
            if(currentBaby.name.equals(girlNames.get(nameCount))){
                name = currentBaby.name;
                femaleFrequency = currentBaby.femaleFreq;
            }
          }

          for(int i = 0; i < babyBoyz.size(); i++){
            Baby currentBaby = babyBoyz.get(i);
            if(currentBaby.name.equals(girlNames.get(nameCount))){
                name = currentBaby.name;
                maleFrequency = currentBaby.maleFreq;
            }
          }

                float percentageGirls;
                float percentageBoys;
                percentageGirls = (float) femaleFrequency * 100 / totalGirls;
                percentageBoys = (float) maleFrequency * 100 / totalMales;
                System.out.println("Name: " + name + ANSI_RED + "  Females: " + femaleFrequency + "  Female Percentage: " + percentageGirls + "%" + ANSI_RESET + ANSI_BLUE + " Males: " + maleFrequency + "  Male Percentage: " + percentageBoys + "%" + ANSI_RESET);
                System.out.println(" ");

                nameCount++;
          }
      }
}
