using Application.UseCases.Pairs.UpdatePair;
using Domain.Entities;
using Domain.RepositoryInterfaces;

namespace UnitTests.Application.UseCases.Pairs;

[TestFixture]
public class UpdatePairTests
{
    private UpdatePairHandler _updatePairHandler = null!;
    private readonly Mock<IPairRepository> _fakePairRepository = new();

    [OneTimeSetUp]
    public void OneTimeSetUp()
    {
        _updatePairHandler = new UpdatePairHandler(_fakePairRepository.Object);
    }

    [SetUp]
    public void Setup()
    {
        _fakePairRepository.Invocations.Clear();
    }

    [TestFixture]
    public class HandleMethod: UpdatePairTests
    {
        [TestCase(null, true)]
        [TestCase(null, false)]
        [TestCase(1, true)]
        [TestCase(1, false)]
        public async Task InputIsValid_ReturnsResponse(int? id, bool isEntityUpdatedSuccessfullyInDb)
        {
            // Arrange
            var request = new UpdatePairRequest(id, new Pair {Id = 1, Name = "Example_Pair_Name_1", Value = "Example_Pair_Value_1"});

            _fakePairRepository
                .Setup(x => x.UpdateAsync(request.Id, request.Pair, It.IsAny<CancellationToken>()))
                .ReturnsAsync(isEntityUpdatedSuccessfullyInDb);

            // Act
            var actual = await _updatePairHandler.Handle(request, default);

            // Assert
            Assert.AreEqual(isEntityUpdatedSuccessfullyInDb, actual.IsSuccess);
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
            var request = new UpdatePairRequest(default, new Pair {Name = pairName, Value = pairValue});

            // Act and Assert
            Assert.ThrowsAsync<ArgumentException>(async () => await _updatePairHandler.Handle(request, default));
            _fakePairRepository.VerifyNoOtherCalls();
        }
    }
}