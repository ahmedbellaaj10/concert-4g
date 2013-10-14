message CustomMessage
{
    int source;
    int destination;
    int hopCount = 0;
    int sizeInBits; // between 1 and 524280
}
