import java.util.*;
import java.io.*;
import java.util.Scanner;
import java.lang.*;

public class BabyRunner {

    public static void main(String[] args){

        //check that the user provided the name of the file
        if(args.length != 1){
            System.err.println("Incorrect amount of arguments");
            System.exit(1);
        }

        String babyFile = args[0];
        System.out.println(" ");

        //initialize data strucutres
        ArrayList<Baby> babyGirls = new ArrayList<Baby>();
        ArrayList<Baby> babyBoys = new ArrayList<Baby>();

        HashMap<String, Baby> babyMap = new HashMap<String, Baby>();

        BinaryTree babyTree = new BinaryTree();


        //initialize scanner and relevant counters
        Scanner console = new Scanner(System.in);

        int linkedListCounter = 0;
        int hashMapCounter = 0;
        int treeCounter = 0;

        //main loop
        for(;;){

            //prompt user fpr data structure and operations
            System.out.println("Please input which data structure you would like to use: \n 1 = Tree \n 2 = Hash Map \n 3 = Linear Data Structure \n");
            String structure = console.next();
            System.out.println("Which task would you like performed? \n 1 = Search for a name \n 2 = Most Popular Names \n 3 = Unique Names \n 4 = Display Names \n");
            String function = console.next();

            String selectedDataStruct = " ";

            //check if data needs to be loaded into data structures and set the selected data structure string
            if((structure.equals("3")) && (linkedListCounter == 0)){
                babyGirls = BabyArrayList.loadGirls(babyFile);
                babyBoys = BabyArrayList.loadBoys();
                selectedDataStruct = "ArrayList";
                linkedListCounter++;

            }else if((structure.equals("3")) && (!(linkedListCounter == 0))){
                selectedDataStruct = "ArrayList";

            }else if((structure.equals("2")) && (hashMapCounter == 0)) {
                babyMap = BabyHashMap.storeBabies(babyFile);
                selectedDataStruct = "HashMap";
                hashMapCounter++;

            } else if((structure.equals("2")) && (!(hashMapCounter == 0))) {
                selectedDataStruct = "HashMap";

            } else if((structure.equals("1")) && (treeCounter == 0)) {
                babyTree = BabyBST.loadBabies(babyFile);
                selectedDataStruct = "Binary Tree";
                treeCounter++;

            } else if((structure.equals("1")) && (!(treeCounter == 0))) {
                selectedDataStruct = "Binary Tree";
            }

            //check which function the user selected and execute the correct one
            if(function.equals("1")){

                System.out.print("Please enter which name you would like to search for:  \n");

                String babyName = console.next();
                System.out.print("\n");


                System.out.println("Selected Data Structure: " + selectedDataStruct);
                System.out.println("Selected Name: " + babyName + "\n");

                if(selectedDataStruct.equals("ArrayList")){
                    BabyArrayList.SearchName(babyName);

                } else if(selectedDataStruct.equals("HashMap")){
                    final long startTime = System.currentTimeMillis();
                    BabyHashMap.SearchName(babyMap, babyName);

                    final long endTime = System.currentTimeMillis();

                    System.out.println("Total execution time: " + (endTime - startTime) + " miliseconds" );

                } else if(selectedDataStruct.equals("Binary Tree")) {
                    BabyBST.searchName(babyTree, babyName);
                }


            }

            if(function.equals("2")){

                System.out.println(" ");
                System.out.println("Selected Data Structure: " + selectedDataStruct);

                if(selectedDataStruct.equals("ArrayList")){
                    BabyArrayList.MostPopularName();

                } else if(selectedDataStruct.equals("HashMap")) {
                    BabyHashMap.MostPopularNames(babyMap);

                } else if(selectedDataStruct.equals("Binary Tree")) {
                    BabyBST.MostPopularNames(babyTree);
                }
            }

            if(function.equals("3")){

                System.out.println(" ");
                System.out.println("Selected Data Structure: " + selectedDataStruct);

                if(selectedDataStruct.equals("ArrayList")){
                    BabyArrayList.UniqueName();
                } else if(selectedDataStruct.equals("HashMap")) {
                    BabyHashMap.UniqueNames(babyMap);
                } else if(selectedDataStruct.equals("Binary Tree")) {
                    BabyBST.UniqueNames(babyTree);
                }
            }

            if(function.equals("4")){

                System.out.println(" ");
                System.out.println("Selected Data Structure: " + selectedDataStruct);

                if(selectedDataStruct.equals("ArrayList")){
                    BabyArrayList.DisplayName();
                } else if(selectedDataStruct.equals("HashMap")) {
                    final long startTime2 = System.currentTimeMillis();
                    BabyHashMap.DisplayNames(babyMap);
                    final long endTime2 = System.currentTimeMillis();

                    System.out.println("Total execution time: " + (endTime2 - startTime2));
                } else if(selectedDataStruct.equals("Binary Tree")) {
                    final long startTime3 = System.currentTimeMillis();
                    BabyBST.display(babyTree.root);
                    final long endTime3 = System.currentTimeMillis();

                    System.out.println("Total execution time: " + (endTime3 - startTime3) + " miliseconds");
                }
            }
        }

    }
}
