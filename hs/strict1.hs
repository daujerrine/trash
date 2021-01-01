import Data.List

avg xs = q / p
    where
        g = foldl' (\(a,b) c -> (a + 1,b + c)) (0,0) xs
        p = fst g
        q = snd g

main :: IO()
main = do
    putStrLn $ show $ avg [0 .. 4000000]
