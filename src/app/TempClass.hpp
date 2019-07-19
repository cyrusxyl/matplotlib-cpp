class TempClass
{
public:
    TempClass() = default;
    virtual ~TempClass() = default;

    int getData();
    void setData(int newData);

private:
    int memberData_{1};
};