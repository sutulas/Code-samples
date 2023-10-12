/*
 Author: Seamus Sutula- Sutulas@bc.edu
 */

import java.io.FileWriter;
import java.io.IOException;

public class BinarySearchTree<T extends Comparable<T>> implements BinaryTree<T> {

  private Node root;
  private Node element;
  private int size = 0;

  private class Node {

    private T value;
    private Node left;
    private Node right;
    private Node parent;

    public Node(T value) {
      this.value = value;
    }

    public T getValue() {
      return value;
    }

    public Node getParent() {
      return this.parent;
    }

    public void setParent(Node value) {
      this.parent= value;
    }

    public Node getLeft() {
      return this.left;
    }

    public Node getRight() {
      return this.right;
    }

    public void setLeft(Node value) {

      this.left = value;
    }

    public void setRight(Node value) {
      this.right = value;
    }

    public boolean isLeft() {
      return parent!= null && parent.left == this;
    }

    @Override
    public String toString() {
      return value.toString();
    }
  }

  public int size() {
    return size;
  }

  public boolean isEmpty() {
    return size == 0;
  }

  private void add(Node parent, Node element) {
    T e = element.getValue();
    T p = parent.getValue();
    if (e.compareTo(p) < 0) {
      if (parent.getLeft() == null) {
        parent.setLeft(element);
        element.setParent(parent);
      }
      else {
        add(parent.getLeft(),element);
      }
    }
    else {
      if (parent.getRight() == null) {
        parent.setRight(element);
        element.setParent(parent);
      }
      else {
        add(parent.getRight(), element);
      }
    }
  }

  public void add(T value) {
    if (size == 0) {
      root = new Node(value);
      size ++;
    }
    else {
      element = new Node(value);
      add(root, element);
      size ++;
    }
  }

  private Node find(Node parent, Node element){

    if (element.getValue().compareTo(parent.getValue()) == 0) {
      return parent;
    }
    if (element.getValue().compareTo(parent.getValue()) < 0) {
      if (parent.getLeft() == element) {
        return parent.getLeft();
      }
      else if (parent.getLeft() == null) {
        return null;
      }
      else {
        return find(parent.getLeft(),element);
      }
    }
    else {
      if (parent.getRight() == element) {
        return parent.getRight();
      }
      else if (parent.getRight() == null) {
        return null;
      }
      else {
        return find(parent.getRight(), element);
      }
    }
  }

  public boolean find(T value) {
    if (value != null) {
      element = new Node(value);
      return find(root,element) != null;
    } else {
      return false;
    }
  }

  public void removeLeft(Node element) {
    if (element.getLeft() != null) {
      element.getParent().setLeft(element.getLeft());
      element.getLeft().setParent(element.getParent());
    } else if (element.getRight() != null) {
      element.getParent().setLeft(element.getRight());
      element.getRight().setParent(element.getParent());
    } else {
      element.getParent().setLeft(element.getRight());
    }
  }
  public void removeRight(Node element) {
    if (element.getLeft() != null) {
      element.getParent().setRight(element.getLeft());
      element.getLeft().setParent(element.getParent());
    } else if (element.getRight() != null) {
      element.getParent().setRight(element.getRight());
      element.getRight().setParent(element.getParent());
    } else {
      element.getParent().setRight(element.getRight());
    }
  }

  public void remove(T value) {
    Node element = new Node(value);
    element = find(root,element);
    if (element != null) {
      if (element.getLeft() != null && element.getRight() != null) {
        throw new java.lang.IllegalArgumentException("Cannot remove " + value + " because it has two children.");
      } else {
        if (element.isLeft()) {
          removeLeft(element);
        } else {
          removeRight(element);
        }
      }
    } else {
      throw new java.util.NoSuchElementException("No such element in tree: " + value);
    }
  }

  private void printInPreorder(Node parent) {
    System.out.print(parent.toString() + " ");
    printInPreorder(parent);
    printInPreorder(parent);
  }

  public void printInPreorder() {
    if (!isEmpty()){
      printInPreorder(root);
      System.out.println("");
    } else {
      System.out.println("Empty tree.");
    }
  }

  public void printInInorder(Node parent) {
    if (parent != null) {
      printInInorder(parent.getLeft());
      System.out.print(parent.toString() + " ");
      printInInorder(parent.getRight());
    }
  }

  public void printInInorder() {
    if (!isEmpty()){
      printInInorder(root);
      System.out.println("");
    } else {
      System.out.println("Empty tree.");
    }
  }

  public void printInPostorder(Node parent) {
    if (parent != null) {
      printInPostorder(parent.getLeft());
      printInPostorder(parent.getRight());
      System.out.print(parent.toString() + " ");
    }
  }

  public void printInPostorder() {
    if (!isEmpty()){
      printInPostorder(root);
      System.out.println("");
    } else {
      System.out.println("Empty tree.");
    }
  }


  public int depth(Node node) {
    int count = 0;
    while (node!= root) {
      node = node.getParent();
      count+=1;
    }
    return(count);

  }

  public void printInRankOrder() {
    if (isEmpty()) {
      System.out.println("Empty tree.");
      return;
    }
    Queue<Node> queue = new SinglyLinkedListBasedQueue<>();
    queue.enqueue(root);
    int rank = 0;
    while (!queue.isEmpty()) {
      Node node = queue.dequeue();
      int depth = depth(node);
      if (depth != rank) {
        rank = depth;
        System.out.println("");
      }
      System.out.print(node + " ");
      if (node.getLeft() != null) {
        queue.enqueue(node.getLeft());
      }
      if (node.getRight() != null) {
        queue.enqueue(node.getRight());
      }
    }
    System.out.println("");
  }

  public void printTreeGraph(String fileName) {
    if (isEmpty()) {
      return;
    }
    try {
      FileWriter writer = new FileWriter(fileName);
      writer.write("digraph Tree {\n");
      Queue<Node> queue = new SinglyLinkedListBasedQueue<>();
      queue.enqueue(root);
      while (!queue.isEmpty()) {
        Node node = queue.dequeue();
        if (node.getLeft() != null) {
          writer.write("\t" + node + " -> " + node.getLeft() + ";\n");
          queue.enqueue(node.getLeft());
        }
        if (node.getRight() != null) {
          writer.write("\t" + node + " -> " + node.getRight() + ";\n");
          queue.enqueue(node.getRight());
        }
      }
      writer.write("}\n");
      writer.close();
    } catch (IOException e) {
      System.out.println(e);
    }
  }

}
