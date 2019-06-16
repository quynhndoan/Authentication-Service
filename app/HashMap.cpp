#include "HashMap.hpp"
#include "HashFunction.hpp"

// Initialize a HashMap
HashMap::HashMap()
{
    hasher = PJWHash;
    totalNumberOfBuckets = initialBucketCount;
    totalNumberOfKeyValuePairs = 0;
    bucket = new Node*[totalNumberOfBuckets];
    createNewBucket(bucket,totalNumberOfBuckets);
}

// Initialize a HashMap
HashMap::HashMap(HashFunction hasher)
{
    this->hasher = hasher;
    totalNumberOfBuckets = initialBucketCount;
    totalNumberOfKeyValuePairs = 0;
    bucket = new Node*[totalNumberOfBuckets];
    createNewBucket(bucket,totalNumberOfBuckets);
}

// Create a HashMap
HashMap::HashMap(const HashMap& hm)
{
    totalNumberOfBuckets = hm.bucketCount();
    bucket = new HashMap::Node*[totalNumberOfBuckets];
    hasher = hm.hasher;
    bucket = new Node*[totalNumberOfBuckets];
    createNewBucket(bucket,totalNumberOfBuckets);
    for (unsigned int hashMapCounter = 0; hashMapCounter < totalNumberOfBuckets; hashMapCounter++)
    {
        Node* head = hm.bucket[hashMapCounter];
        if (head == nullptr)
        {
            bucket[hashMapCounter] = nullptr;
            continue;
        }
        else
        {
            while (head != nullptr)
            {
                add(hm.bucket[hashMapCounter]->key, hm.bucket[hashMapCounter]->value);
                head = head->next;
            }
        }
    }
}

// Delete a HashMap
HashMap::~HashMap()
{
    deleteBucket(bucket, totalNumberOfBuckets);
}

// Copy a HashMap
HashMap& HashMap::operator=(const HashMap& hm)
{
    if (this != &hm)
    {
        hasher = hm.hasher;
        HashMap::Node** oldBucket = bucket;
        unsigned int oldTotalNumberOfBuckets = totalNumberOfBuckets;
        totalNumberOfBuckets = hm.totalNumberOfBuckets;
        totalNumberOfKeyValuePairs = 0;
        bucket = new Node*[totalNumberOfBuckets];
        createNewBucket(bucket,totalNumberOfBuckets);
        for (unsigned int hashMapCounter = 0; hashMapCounter < totalNumberOfBuckets; hashMapCounter++)
        {
            HashMap::Node* entry = bucket[hashMapCounter];
            if (entry == nullptr)
            {
                bucket[hashMapCounter] = nullptr;
                continue;
            }
            else
            {
                while (entry != nullptr)
                {
                    add(entry->key, entry->value);
                    entry = entry->next;
                }
            }
        }
        deleteBucket(oldBucket, oldTotalNumberOfBuckets);
    }
    return *this;
}

void HashMap::add(const std::string& key, const std::string& value)
{
    unsigned int hashValue = hashCode(key);
    HashMap::Node* entry = bucket[hashValue];
    HashMap::Node* head = entry;

    // Check if new entry is not a null pointer and already exsist in the bucket
    while (entry != nullptr)
    {
        if (entry->key != key)
        {
            return;
        }
        else
        {
            head = entry;
            entry->next = head;
        }
    }

    HashMap::Node* newEntry = new HashMap::Node;
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = nullptr;
    // Add new entry to an empty list
    if (head == nullptr)
    {
        bucket[hashValue] = newEntry;
    }
    else
    {
        head->next = newEntry;
    }

    totalNumberOfKeyValuePairs++;
    this->~HashMap();
    double currentLoad = loadFactor();
    if (currentLoad > 0.8)
    {
        reHash();
    }
}

void HashMap::remove(const std::string& key)
{
    unsigned int hashValue = hashCode(key);
    HashMap::Node* previous = nullptr;
    HashMap::Node* entry = bucket[hashValue];

    while (entry != nullptr && entry->key != key)
    {
        previous = entry;
        entry = entry->next;
    }

    if (entry == nullptr)
    {
        return;
    }
    else
    {
        if (previous == nullptr)
        {
            bucket[hashValue] = entry->next;
        }
        else
        {
            previous->next = entry->next;
        }
        delete entry;
        totalNumberOfKeyValuePairs--;
    }

}

bool HashMap::contains(const std::string& key) const
{
    bool answer = false;
    unsigned int hashValue = hashCode(key);
    HashMap::Node* entry = bucket[hashValue];
    while (entry != nullptr)
    {
        if (entry->key == key)
        {
            answer = true;
            break;
        }
        else
        {
            entry = entry->next;
        }
    }
    return answer;
}

std::string HashMap::value(const std::string& key) const
{
    unsigned int hashValue = hashCode(key);
    HashMap::Node* entry = bucket[hashValue];
    while (entry != nullptr)
    {
        if (entry->key == key)
        {
            std::string value = entry->value;
            return value;
        }
        entry = entry->next;
    }
    return "";
}


unsigned int HashMap::size() const
{
    return totalNumberOfKeyValuePairs;
}


unsigned int HashMap::bucketCount() const
{
    return totalNumberOfBuckets;
}

double HashMap::loadFactor() const
{
    double loadFactor;
    loadFactor = (double)size() / (double)bucketCount();
    return loadFactor;
}


unsigned int HashMap::maxBucketSize() const
{
    unsigned int maxBucketSize = 0;
    unsigned int pairCounter;
    for (unsigned int maxBucketSizeCounter = 0; maxBucketSizeCounter < totalNumberOfBuckets; maxBucketSizeCounter++)
    {
        pairCounter = 0;
        HashMap::Node* entry = bucket[totalNumberOfBuckets];
        while(entry != nullptr)
        {
            pairCounter++;
            entry = entry->next;
        }
        if (pairCounter > maxBucketSize)
        {
            maxBucketSize = pairCounter;
        }
    }
    return maxBucketSize;
}

void HashMap::createNewBucket(Node** &bucket, unsigned totalNumberOfBuckets)
{
    // bucket = new HashMap::Node*[totalNumberOfBuckets];
    for (unsigned int i = 0; i < totalNumberOfBuckets; i++)
    {
        bucket[i] = nullptr;
    }
}

// Deallocate the entire bucket, start at the first entry
void HashMap::deleteBucket(HashMap::Node** bucket, unsigned int totalNumberOfBuckets)
{
    for (unsigned int i = 0; i < totalNumberOfBuckets; i++)
    {
        HashMap::Node* entry = bucket[i];
        if (entry == nullptr)
        {
            return;
        }
        HashMap::Node* head = entry;
        HashMap::Node* next = head->next;
        while (entry != nullptr)
        {
            next = head;
            head = head->next;
            delete next;
        }
    }
    delete[] bucket;
}


unsigned int HashMap::hashCode(const std::string& key) const
{
    unsigned int hashKey = hasher(key);
    unsigned int hashCode =  hashKey % totalNumberOfBuckets;
    return hashCode;
}

void HashMap::reHash()
{
    HashMap::Node** oldBucket = bucket;
    unsigned int oldTotalNumberOfBuckets = totalNumberOfBuckets;
    unsigned int newTotalNumberOfBuckets = totalNumberOfBuckets * 2 + 1;
    bucket = new HashMap::Node*[newTotalNumberOfBuckets];
    for (unsigned int newTotalNumberOfBucketsIndex = 0; newTotalNumberOfBucketsIndex < newTotalNumberOfBuckets; newTotalNumberOfBucketsIndex++)
    {
        bucket[newTotalNumberOfBucketsIndex] = nullptr;
    }
    for (int oldTotalNumberOfBucketsIndex = 0; oldTotalNumberOfBucketsIndex < oldTotalNumberOfBuckets; oldTotalNumberOfBucketsIndex++)
    {
        HashMap::Node* oldNode = oldBucket[oldTotalNumberOfBucketsIndex];
        if (oldNode == nullptr)
        {
            continue;
        }
        HashMap::Node* temp = oldNode->next;
        while (oldNode != nullptr)
        {
            add(oldNode->key, oldNode->value);
            temp = oldNode;
            oldNode = oldNode->next;
            delete temp;
        }
    }
    delete[] oldBucket;
}

// memory leak in add function, createNewBucket
