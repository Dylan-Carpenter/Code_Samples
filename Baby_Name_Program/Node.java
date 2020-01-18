import java.util.*;

//basic node class for tree
public class Node {

  private Node parent;
  private Node left;
  private Node right;
  private Baby baby;
  private int heightLeft;
  private int heightRight;

  public Node() {
    this.parent = null;
    this.left = null;
    this.right = null;
    this.baby = null;
    this.heightLeft = 0;
    this.heightRight = 0;
  }


  public Node(Baby initBaby) {
    this.parent = null;
    this.left = null;
    this.right = null;
    this.baby = initBaby;
    this.heightLeft = 0;
    this.heightRight = 0;

  }

  public void setLeft(Node left){
     this.left = left;
   }

   public void setRight(Node right){
     this.right = right;
   }

   public void setParent(Node parent){
     this.parent = parent;
   }

   public void setLeftHeight(int heightL) {
     this.heightLeft = heightL;
   }

   public void setRightHeight(int heightR) {
     this.heightRight = heightR;
   }

   public Node getLeft(){
     return left;
   }

   public Node getRight(){
     return right;
   }

   public Node getParent(){
     return parent;
   }

   public Baby getBaby() {
     return baby;
   }

   public int getLeftHeight() {
     return heightLeft;
   }

   public int getRightHeight() {
     return heightRight;
   }

}
