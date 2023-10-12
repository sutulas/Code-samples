/*
 * Author: Seamus Sutula - sutulas@bc.edu
 */

public interface MapEntry<K, V> {

  K getKey();
  V getValue();
  void setValue(V value);
}
