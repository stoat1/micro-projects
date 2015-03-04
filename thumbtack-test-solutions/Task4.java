/*
 * 4. Подсчитать количество строк заданной целочисленной матрицы 20x20,
 * являющихся перестановкой чисел 1, 2, ..., 20
 */

package thumbtacktasks;

/**
 * @author Евгений Трапезников
 */
public class Task4 
{    
    /**
     * The solution of the task
     * @param matrix - is integer matrix
     * @return count of rows which are premutation of 1, 2, ..., 20
     */
    public static int solution(int[][] matrix) {
        int count = 0;        
        
        rowsLoop:
        for(int[] row : matrix)
        {
            // info about encountered values is stored in 1, 2, ..., 20 bits
            int mask = 0;
            
            for(int elem : row)
            {
                if((elem < 1) || (20 < elem))
                {
                    continue rowsLoop;
                }
                
                mask |= 1 << elem;                            
            }
            
            // 0x001ffffe + 2 is 0x00200000
            if(mask + 2 == 1 << (20 + 1))
            {
                ++count;
            }
        }
        
        return count;
    }
}
