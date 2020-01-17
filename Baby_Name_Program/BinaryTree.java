import java.util.*;

public class BinaryTree {

  public Node root;
  private int height;
  private int size;
  private int depth;


  public BinaryTree() {
    Node root = null;
    int height = 0;
    int size = 0;
    int depth = 0;
  }



    public Baby getBaby(Node root, String name) {

      Baby baby = new Baby();

      Node current = root;

      while(current != null) {
        if ((name.compareTo(current.getBaby().name) < 0)) {
          current = current.getLeft();
        } else if(name.compareTo(current.getBaby().name) > 0) {
          current = current.getRight();
        } else {
          baby = current.getBaby();
          current = null;
        }
      }

      return baby;

    }



  public void insertBabyName(Baby baby) {

    Node newNode = new Node(baby);

    if(root == null){
      root = newNode;
    }else{

      Node parent = root;
      Node current = root;

      while(current != null){

        parent = current;

        if((baby.name.compareTo(current.getBaby().name) < 0)){
          current = current.getLeft();

          if(parent.getRight() == null){
            parent.setLeftHeight(parent.getLeftHeight() + 1);
          }


        }else if(baby.name.compareTo(current.getBaby().name) > 0){
          current = current.getRight();

          if(parent.getLeft() == null){
            parent.setRightHeight(parent.getRightHeight() + 1);
          }

        }else{

          parent.getBaby().maleFreq = baby.maleFreq;
          parent.getBaby().maleRank = baby.maleRank;
          current = null;
          return;
        }
      }

      if(baby.name.compareTo(parent.getBaby().name) < 0){
        parent.setLeft(newNode);
      }else{
        parent.setRight(newNode);
      }
    }
  }

  public void rotateRight(){

    Node newLeft = null;

    if(root.getLeft().getRight() != null){
      newLeft = root.getLeft().getRight();
    }

    root.setParent(root.getLeft());

    root.getLeft().setRight(root);

    if(newLeft != null){
      root.setLeft(newLeft);
    }

    root = root.getParent();
  }

  public void rotateLeft(){

    Node newRight = null;

    if(root.getRight().getLeft() != null){
      newRight = root.getRight().getLeft();
    }

    root.setParent(root.getRight());

    root.getRight().setLeft(root);

    if(newRight != null){
      root.setRight(newRight);
    }

    root = root.getParent();
  }

  public void insertMaleFreq(Node root, Baby baby) {


  }

  public void insertFemaleFreq(Node root, Baby baby) {

  }


  public void deleteBaby() {


  }










}
