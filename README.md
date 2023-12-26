# Linked-list-snake
Snake game based on Linked-List, C
Fps Setting                   
使用Sleep()依據玩家需求設定難度。

Snake Body (Linked-list Node)
每個Node中包含x座標與y座標並指向下一個Body。

Snake Tail(Head of Linked-list)
蛇以Tail作為Linked-list的head，方便後續操作

Snake Move                   
透過在List尾端新增Node，
並刪除List Head實現蛇的移動。


Direction Change          
使用_kbhit()、_getch()擷取玩家輸入按鍵。
並依據輸入的按鍵改變蛇的移動方向。


Demo
![含方向](https://github.com/SuChenYao/Linked-list-snake/assets/102650897/8f338849-cbf9-4b21-9d61-676537fde5bb)

