#include <iostream>
#include <unordered_map>
#include <list>

/*
 * LRUキャッシュの実装
 * - 最近使われたデータをキャッシュに保持し、古いものから破棄する
 * - put/get ともに O(1) で動作
 * 
 * 実用性のあるデータ構造サンプル
 */

class LRUCache {
private:
    int capacity;
    std::list<std::pair<int, int>> items; // {key, value}
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;

        // アクセスされた要素をリストの先頭へ移動
        items.splice(items.begin(), items, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = cache.find(key);

        if (it != cache.end()) {
            // 既存要素を更新＆先頭へ移動
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
        } else {
            if ((int)items.size() == capacity) {
                // 一番古い要素を削除
                auto last = items.back();
                cache.erase(last.first);
                items.pop_back();
            }
            // 新しい要素を先頭へ追加
            items.emplace_front(key, value);
            cache[key] = items.begin();
        }
    }

    void display() {
        std::cout << "Cache state: ";
        for (auto& p : items) {
            std::cout << "[" << p.first << ":" << p.second << "] ";
        }
        std::cout << "\n";
    }
};

int main() {
    LRUCache lru(3);

    lru.put(1, 100);
    lru.put(2, 200);
    lru.put(3, 300);
    lru.display();

    std::cout << "get(2) = " << lru.get(2) << "\n"; // キャッシュヒット
    lru.display();

    lru.put(4, 400); // (1,100) が削除される
    lru.display();

    std::cout << "get(1) = " << lru.get(1) << "\n"; // キャッシュミス → -1
    std::cout << "get(3) = " << lru.get(3) << "\n"; // キャッシュヒット
    lru.display();

    return 0;
}
