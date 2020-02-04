
import java.util.*;
import java.io.*;
import java.util.Scanner;
import java.text.*;

public class BabyBST {

  //colors used to style output
  public static final String ANSI_RED = "\u001B[35m";
  public static final String ANSI_RESET = "\u001B[0m";
  public static final String ANSI_BLUE = "\u001B[34m";

  //initialize variables
  public static Baby[] topMale = new Baby[10];
  public static Baby[] topFemale = new Baby[10];
  public static int numFemale = 0;
  public static int numMale = 0;

  public static ArrayList<Baby> uniqueMale = new ArrayList<Baby>();
  public static ArrayList<Baby> uniqueFemale = new ArrayList<Baby>();

  public static int totalBabies = 0;

  public static BinaryTree babyTree = new BinaryTree();

  //loads all babies into a binary tree
  public static BinaryTree loadBabies(String babyFile) {


    try {
      Scanner input = new Scanner(new File(babyFile));
      int femaleRank = 0;
      int maleRank = 0;
      int maleUniqueCounter = 0;
      int femaleUniqueCounter = 0;

      //reads file
      while (input.hasNextLine()) {
        String[] babyArray = input.nextLine().split(",");

        //adds female baby
        if (babyArray[1].equals("F")) {

          femaleRank++;
          Baby baby = new Baby(babyArray[0], Integer.parseInt(babyArray[2]), 0, femaleRank, 0);
          babyTree.insertBabyName(baby);
          numFemale = numFemale + baby.femaleFreq;
          totalBabies = totalBabies + baby.femaleFreq;

          //keeps track of most popular girl names
          if (femaleRank <= 10) {
            topFemale[femaleRank-1] = baby;
          }
          //keeps track of unique female names
          if (baby.femaleFreq == 5 && femaleUniqueCounter < 5){
            uniqueFemale.add(baby);
            femaleUniqueCounter++;
          }

          //adds male baby
        } else {


              maleRank++;
              Baby baby = new Baby(babyArray[0], 0, Integer.parseInt(babyArray[2]), 0, maleRank);
              babyTree.insertBabyName(baby);
              numMale = numMale + baby.maleFreq;
              totalBabies = totalBabies + baby.maleFreq;

              //keeps track of most popular boy names
              if (baby.maleRank <= 10) {
                topMale[maleRank-1] = baby;
              }
              //keeps track of unique boy names
              if(baby.maleFreq == 5 && maleUniqueCounter < 5){
                uniqueMale.add(baby);
                maleUniqueCounter++;
              }
            }

        }


        }catch (FileNotFoundException e) {
          System.out.println("File does not exist");
          System.exit(0);

      }

      return babyTree;

    }

    //displays all names in alphabetic order
    public static void display(Node ro) {

      Node currNode = ro;

      //recursivley goes through each node and grabs the baby associated with it and prints. (since they're already sorted via names)
      if (currNode != null) {
        display(currNode.getLeft());
        DecimalFormat df = new DecimalFormat("#.####");
        int sum = currNode.getBaby().maleFreq + currNode.getBaby().femaleFreq;
        float percent = (float)sum * 100 / totalBabies;
        System.out.println(currNode.getBaby().name + "  " + sum + "    " + df.format(percent));

        display(currNode.getRight());

      }


    }

    //searchs through the tree for a particular name
    public static void searchName(BinaryTree tree, String name) {

      //goes and get the specified baby
      Baby baby = tree.getBaby(tree.root, name);

      //prints that babies info
      System.out.println(ANSI_BLUE + "Male Frequency: " + baby.maleFreq + " Rank: " + baby.maleRank + ANSI_RESET);
      System.out.println(ANSI_RED + "Female Frequency: " + baby.femaleFreq + " Rank: " + baby.femaleRank  + "\n\n\n" + ANSI_RESET);


    }

    //prints out the ten most popular names of each gender
    public static void MostPopularNames(BinaryTree tree) {

      System.out.println();
      System.out.println(ANSI_RED + "Top 10 Female Baby Names:" + ANSI_RESET);
      System.out.println();

      DecimalFormat df = new DecimalFormat("#.##");

      // Prints the top 10 female baby Names
      for (Baby baby: topFemale) {
        float percent = (float)baby.femaleFreq * 100 / numFemale;

        System.out.println(ANSI_RED + "Name: " + baby.name + " Frequency: " + baby.femaleFreq + " Percentage: " +  df.format(percent) + "%" + ANSI_RESET);
      }

      System.out.println();
      //prints the top ten male baby names
      System.out.println(ANSI_BLUE + "Top 10 Male Baby Names:" + ANSI_RESET);
      System.out.println();
      for (Baby baby: topMale) {
        float percent = (float)baby.maleFreq * 100 / numMale;
        System.out.println(ANSI_BLUE + "Name: " + baby.name + " Frequency: " + baby.maleFreq + " Percentage: " + df.format(percent) + "%" + ANSI_RESET);
      }

    }

    //displays the first names that have a frequency of just 5
    public static void UniqueNames(BinaryTree tree) {

      float malePercentage = (float) 500 / numMale;
      float femalePercentage = (float) 500 / numFemale;

      System.out.println(ANSI_BLUE + "Male: " + ANSI_RESET);
      System.out.println("");

      //prints out the info of uniqueMale
      for (Baby baby: uniqueMale) {
        System.out.println(ANSI_BLUE + "Name: " + baby.name + " Frequency: " + 5 + " Percentage: " + malePercentage + "%" + ANSI_RESET);
      }

      System.out.println("");
      System.out.println(ANSI_RED + "Female: " + ANSI_RESET);
      System.out.println("");

      //prints out the info of uniqueFemale
      for (Baby baby: uniqueFemale) {
        System.out.println(ANSI_RED + "Name: " + baby.name + " Frequency: " + 5 + " Percentage: " + femalePercentage + "%" + ANSI_RESET);
      }

    }







}
