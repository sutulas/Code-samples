/*
 * Author: Seamus Sutula -- sutulas@bc.edu
 */

import java.util.Scanner;
import java.util.Arrays;
import java.util.Random;
import java.util.ArrayList;
import java.util.Iterator;

public class HashTable<K extends Comparable<K>, V> implements Map<K, V> {

  public static class Entry<K extends Comparable<K>, V> implements MapEntry<K, V>, Comparable<Entry<K, V>> {

    private K key;
    private V value;

    public Entry(K key, V value) {
      this.value = value;
      this.key = key;
    }
    @Override
    public K getKey() {
      return key;
    }
    @Override
    public V getValue() {
      return value;
    }
    @Override
    public void setValue(V value) {
      this.value = value;
    }

    public int compareTo(Entry<K, V> object) {
      return this.key.compareTo(object.key);
    }

    public String toString() {
      return key.toString() + ": " + value.toString();
    }
  }

  private class KeyIterator implements Iterator<K> {

    private Iterator<MapEntry<K, V>> entries = entrySet().iterator();

    @Override
    public K next() {
      return entries.next().getKey();
    }

    @Override
    public boolean hasNext() {
      return entries.hasNext();
    }
  }

  private class KeyIterable implements Iterable<K> {

    @Override
    public Iterator<K> iterator() {
      return new KeyIterator();
    }
  }

  private class ValueIterator implements Iterator<V> {

    private Iterator<MapEntry<K, V>> entries = entrySet().iterator();

    @Override
    public V next() {
      return entries.next().getValue();
    }

    @Override
    public boolean hasNext() {
      return entries.hasNext();
    }
  }

  private class ValueIterable implements Iterable<V> {
    @Override
    public Iterator<V> iterator() {
      return new ValueIterator();
    }
  }

  private enum Resize { UP, DOWN };
  private static final int INITIAL_CAPACITY = 17;
  private static final double MAX_LOAD_FACTOR = 0.7;
  private static final double MIN_LOAD_FACTOR = 0.2;
  private static final int PRIME = 109345121;
  private ArrayList<Entry<K, V>>[] table;
  public int capacity;
  private int scale;
  private int shift;
  private int size;
  private double loadFactor;

  public HashTable(boolean repeatable) {
    Random random = repeatable ? new Random(1) : new Random();
    scale = random.nextInt(PRIME - 1) + 1;
    shift = random.nextInt(PRIME);
    capacity = INITIAL_CAPACITY;
    table = createTable();

  }

  public HashTable() {
    this(false);
  }

  @SuppressWarnings("unchecked")
  private ArrayList<Entry<K, V>>[] createTable() {
    size = 0;
    ArrayList<Entry<K, V>>[] newTable = (ArrayList<Entry<K, V>>[]) new ArrayList[capacity];
    return newTable;
  }

  public V get(K key) {
    int index = findIndex(key);
    if (isEmpty()) {
      return null;
    }
    if (table[index] != null) {
      for (Entry<K, V> entry : table[index]) {
        if (entry.getKey().compareTo(key) == 0) {
          return entry.getValue();
        }
      }

    }
    return null;
  }

  private V put(K key, V value, ArrayList<Entry<K, V>>[] table) {
    int index = findIndex(key);
    if (table[index] != null) {
      for (Entry<K, V> entry : table[index]) {
        if (entry.getKey().compareTo(key) == 0) {
          V oldEntry = entry.getValue();
          entry.setValue(value);
          return oldEntry;
        }
      }
    } else {
      table[index] = new ArrayList<Entry<K, V>>();
    }
    table[index].add(new Entry<K, V>(key, value));
    ++size;
    loadFactor = (double) size / (double) capacity;
    return null;
  }

  @Override
  public V put(K key, V value) {
    V rtrn = put(key, value, table);
    if (loadFactor > MAX_LOAD_FACTOR) {
      resizeTable(Resize.UP);
    }
    return(rtrn);
  }

  public Iterable<MapEntry<K, V>> entrySet() {
    ArrayList<MapEntry<K, V>> newEntry = new ArrayList<>();
    for (int i = 0; i < capacity; ++i) {
      if (table[i] != null) {
        for (Entry<K, V> entry : table[i]) {
          newEntry.add(entry);
        }
      }
    }
    return newEntry;
  }

  public int size() {
    return size;
  }

  @Override
  public V remove(K key) {
    int index = findIndex(key);
    ArrayList<Entry<K, V>> list = table[index];
     if (list == null) {
       return null;
     }

     for (int i = 0; i < list.size(); i++) {
       if (list.get(i).getKey().compareTo(key) == 0) {
         V answer = list.remove(i).getValue();
         --size;
         loadFactor = (double) size / (double) capacity;
         if (list.size() ==0) {
           table[index] = null;
         } else if (loadFactor < MIN_LOAD_FACTOR) {
           resizeTable(Resize.DOWN);
         }
         return answer;
       }
     }
     return null;
  }

  @Override
  public Iterable<K> keySet() {
    return new KeyIterable();
  }

  @Override
  public Iterable<V> values() {
    return new ValueIterable();
  }

  @Override
  public boolean isEmpty() {
    return size == 0;
  }

  private int findIndex(K key) {
    return (Math.abs((key.hashCode()) * scale + shift) % PRIME) % capacity;
  }

  private void resizeTable(Resize direction) {

    if (direction == Resize.UP) {
      capacity *= 2;
    } else if (capacity>2) {
      capacity /= 2;
    }
    int primeCapacity = 2;
    PrimeFinder primes = new PrimeFinder((int) Math.ceil(capacity * 1.5));

    while (primes.hasNext() && primeCapacity < capacity) {
      primeCapacity = primes.next();
    }

    capacity = primeCapacity;
    ArrayList<Entry<K, V>>[] newTable = createTable();

    for (ArrayList<Entry<K, V>> list : table) {
      if (list != null) {
        for (Entry<K, V> entry : list) {
          put(entry.getKey(), entry.getValue(), newTable);
        }
      }
    }

    table = newTable;
  }
}
