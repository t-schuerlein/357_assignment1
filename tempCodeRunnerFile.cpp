  if(argc < 5){
        printf("not enough arguments\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    FILE *file2 = fopen(argv[2], "rb");
    FILE *res = fopen(argv[4], "wb");

    if(!file){
        printf("could NOT open image 1\n");
        return 1;
    }
    if(!file2){
        printf("could NOT open image 2\n");
        return 1;
    }
    if(!res){
        printf("could NOT open output file");
        return 1;
    }

    float ratio1 = 0.9;