# Step 1
- まさにスタックの出番ですね，という感想
    - 時間計算量O(N), 空間計算量O(N).
```c++
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        stack<int> nodes;
        // stackに値を格納．
        for (; head; head = head->next) {
            nodes.push(head->val);
        }

        // 一つずつpopしていけば入れたのと逆順で取り出せる．
        ListNode dummy;
        ListNode* tail = &dummy;
        while (!nodes.empty()) {
            tail->next = new ListNode(st.top());
            nodes.pop();
            tail = tail->next;
        }
        
        return dummy.next;
    }
};
```
- 新しくノード作成せず，与えられたノードを用いて再構築すると以下のコード．
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        stack<ListNode*> nodes;
        
        for (ListNode* node = head; node; node = node->next) {
            nodes.push(node);
        }

        ListNode dummy;
        ListNode* tail = &dummy;
        while (!nodes.empty()) {
            tail->next = nodes.top();
            tail = tail->next;
            tail->next = nullptr;
            nodes.pop();
        }

        return dummy.next;
    }
};
```
- 修正したのが以下のコード
# Step 2
- 再帰でもかけるらしいので書いてみる
  - 各ノードを引数に取る`reverseListHelper(ListNode* node)`を定義する．
  - 再帰呼び出しの時に`node->next`を渡したら，`node->next`以降のノードが逆順で繋がったLinkedList*型の変数`new_node`が帰ってほしい．
  - それが帰ってきたら，`new_node->next`に自分の値(`node->val`)を値にもつノードをくっつけて，そのノードのアドレスを返せば再帰のサイクルができる
  - base caseを考える．上のサイクルが止まるのは`node->next`が存在しないとき，つまり`node == nullptr`の時で，この時は&sentinelを返せば良い．
  - 計算量は時間空間ともにO(N).
      - `new ListNode(...)`で新しいNodeを作らずに，入力のリストを破壊して繋ぎ直せば空間計算量は定数に抑えられる．
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        reverseListHelper(head);
        return sentinel.next;
    }

private:
    ListNode sentinel;

    ListNode* reverseListHelper(ListNode* tail_unreversed) {
        if (!tail_unreversed) {
            return &sentinel;
        }

        ListNode* new_tail = reverseListHelper(tail_unreversed->next);
        new_tail->next = new ListNode(tail_unreversed->val);
        return new_tail->next;
    }
};
```
- 以上の再帰は**今のノードより（入力リスト上でみて）後ろのノードたちの整理が終わった状態で，自分を加えて前の人に渡す**というスタイル．
    - 上の人から呼ばれたら，下の人に仕事を振って，途中まで完成したのが帰ってきたら自分の仕事をして，上の人に渡す．
- **今のノードより前のノードの整理が終わった状態で，後ろのノードに渡す**というスタイルもあるらしいので，書いてみる．
    - 上の人から呼ばれたら，上の人から途中まで完成したのをもらって，自分の仕事をしてから，下の人に仕事を振る．
- 上の整理の仕方は返り値が`void`と`ListNode*`とで揃っていないので綺麗な分類になっていないが．まあ自分の仕事をしてから仕事を振るか，仕事を振ってから自分の仕事をするかの違いということ
- 再帰の返り値について，どっちがいいのだろうか？自分なりにまとめると：
    - voidということは，基本的にグローバル的な領域で作業するということ．オフィスにでかい机を用意して，みんなで順番に積み木を載せていくイメージ．
        - 全体に対してどの地点からでも柔軟な操作ができそう，作業効率が良さそう（速そう）なのに対して，誰かヘマをやらかすと全体に影響する．気付かぬうちにミスが紛れ込む可能性が大きそう．
    - 返り値を用いるのは，基本的にそれぞれのローカルな領域で作業するということ．オフィスに個人個人の机を用意して，自分の積み木ができたら隣の人に渡していくイメージ．
        - どこでエラーがあったかわかりやすそうだが，受け渡しの時間がかかるのと，全体に反映させる操作がしにくそう．
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        reverseListHelper(head, nullptr);
        return sentinel.next;
    }

private:
    ListNode sentinel;

    void reverseListHelper(ListNode* node, ListNode* reversed_head) {
        if (!node) {
            sentinel.next = reversed_head;
            return;
        }

        ListNode* next = node->next;
        node->next = reversed_head;
        reverseListHelper(next, node);
    }
};
```
- 再帰をループに直してみる．
    - 「上の人から呼ばれたら，上の人から途中まで完成したのをもらって，自分の仕事をしてから，下の人に仕事を振る」の方は簡単にループにできる．もう一方の再帰は無理そう．
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* head_reversed = nullptr;
        ListNode* head_unreversed = head;
        while (head_unreversed) {
            ListNode* next = head_unreversed->next;
            head_unreversed->next = head_reversed;

            // a <- b <- head_reversed | head_unreversed -> next -> d　が，
            // a <- b <- head_reversed <- head_unreversed | next -> d　になったので，ポインタを1個ずらす．
            head_reversed = head_unreversed;
            head_unreversed = next;
        }
        return head_reversed;
    }
};
```
- ついでに，繋ぎかえでなくて新しくノードを作成するバージョンも．
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* head_unreversed = head;
        ListNode* head_reversed = nullptr;
        while (head_unreversed) {
            head_reversed = new ListNode(head_unreversed->val, head_reversed);
            head_unreversed = head_unreversed->next;
        }
        return head_reversed;
    }
};
```
  
