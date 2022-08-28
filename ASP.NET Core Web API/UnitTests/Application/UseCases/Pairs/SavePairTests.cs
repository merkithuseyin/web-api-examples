using Application.UseCases.Pairs.SavePair;
using Domain.Entities;
using Domain.RepositoryInterfaces;

namespace UnitTests.Application.UseCases.Pairs;

[TestFixture]
public class SavePairTests
{
    private SavePairHandler _savePairHandler = null!;
    private readonly Mock<IPairRepository> _fakePairRepository = new();

    [OneTimeSetUp]
    public void OneTimeSetUp()
    {
        _savePairHandler = new SavePairHandler(_fakePairRepository.Object);
    }

    [SetUp]
    public void Setup()
    {
        _fakePairRepository.Invocations.Clear();
    }

    [TestFixture]
    public class HandleMethod: SavePairTests
    {
        [TestCase(true)]
        [TestCase(false)]
        public async Task InputIsValid_ReturnsResponse(bool isEntitySavedSuccessfullyInDb)
        {
            // Arrange
            var request = new SavePairRequest(new Pair {Name = "Example_Pair_Name_1", Value = "Example_Pair_Value_1"});

            _fakePairRepository
                .Setup(x => x.SaveAsync(request.Pair, It.IsAny<CancellationToken>()))
                .ReturnsAsync(isEntitySavedSuccessfullyInDb);

            // Act
            var actual = await _savePairHandler.Handle(request, default);

            // Assert
            Assert.AreEqual(isEntitySavedSuccessfullyInDb, actual.IsSuccess);
        }

        
        [TestCase("Example_Pair_Name_1", null)]
        [TestCase("Example_Pair_Name_1", "")]
        [TestCase("Example_Pair_Name_1", " ")]
        
        [TestCase(null, "Example_Pair_Value_1")]
        [TestCase("", "Example_Pair_Value_1")]
        [TestCase(" ", "Example_Pair_Value_1")]
        
        [TestCase(null, null)]
        [TestCase(null, "")]
        [TestCase(null, " ")]

        [TestCase("", null)]
        [TestCase("", "")]
        [TestCase("", " ")]
        
        [TestCase(" ", null)]
        [TestCase(" ", "")]
        [TestCase(" ", " ")]
        public void InputIsNotValid_ThrowsException(string pairName, string pairValue)
        {
            // Arrange
            var request = new SavePairRequest(new Pair {Name = pairName, Value = pairValue});

            // Act and Assert
            Assert.ThrowsAsync<ArgumentException>(async () => await _savePairHandler.Handle(request, default));
            _fakePairRepository.VerifyNoOtherCalls();
        }
    }
}